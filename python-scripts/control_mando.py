import pygame
import socket
import time
import sys

# --- CONFIGURACIÓN ---
BT_MAC = '00:25:03:01:6F:C5'  # Dirección MAC de tu HC-05
BT_PORT = 1                   # Puerto/Canal estándar para HC-05
SEND_INTERVAL = 0.01          # Intervalo de envío en segundos (100Hz - Máxima velocidad recomendada)

def detect_axes(mando):
    """Función para ayudar a identificar qué eje es cuál."""
    print("\n--- Modo de detección de ejes ---")
    print("Mueve los sticks y gatillos para ver sus índices.")
    print("Presiona Ctrl+C para comenzar la transmisión normal.\n")
    try:
        while True:
            pygame.event.pump()
            outputs = []
            for i in range(mando.get_numaxes()):
                val = mando.get_axis(i)
                if abs(val) > 0.1: # Filtro de ruido
                    outputs.append(f"Eje {i}: {val:>6.2f}")
            if outputs:
                print(" | ".join(outputs), end='\r')
            time.sleep(0.1)
    except KeyboardInterrupt:
        print("\n\nIniciando transmisión de datos...")

def main():
    # 1. Inicializar Bluetooth Socket
    bt_sock = None
    try:
        bt_sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
        bt_sock.connect((BT_MAC, BT_PORT))
        print(f"Conectado exitosamente al HC-05 ({BT_MAC})")
    except Exception as e:
        print(f"ADVERTENCIA: No se pudo conectar al Bluetooth {BT_MAC}. ({e})")
        print("El script continuará en modo solo consola (DEBUG).\n")

    # 2. Inicializar Pygame y Joystick
    pygame.init()
    pygame.joystick.init()

    if pygame.joystick.get_count() == 0:
        print("ERROR: No se encontró ningún mando conectado. Conecta uno y vuelve a intentar.")
        return

    mando = pygame.joystick.Joystick(0)
    mando.init()
    print(f"Mando detectado: {mando.get_name()}")
    print(f"Número de ejes: {mando.get_numaxes()}")

    # --- ASIGNACIÓN DE EJES ---
    INDEX_JOYSTICK = 0
    INDEX_GATILLO = 5 

    print(f"\nTransmitiendo datos (Gatillo Eje {INDEX_GATILLO}, Joystick Eje {INDEX_JOYSTICK})...")
    print("Presiona Ctrl+C para salir.\n")

    loop_counter = 0
    try:
        while True:
            pygame.event.pump()

            # 3. Leer valores
            val_joystick = mando.get_axis(INDEX_JOYSTICK)
            val_gatillo = mando.get_axis(INDEX_GATILLO)

            # 4. Normalización para protocolo de 8 bits (0-255)
            gatillo_byte = int(round(((val_gatillo + 1) / 2) * 255))
            joystick_byte = int(round(((val_joystick + 1) / 2) * 255))

            gatillo_byte = max(0, min(255, gatillo_byte))
            joystick_byte = max(0, min(255, joystick_byte))

            # 5. Crear paquete binario (2 bytes)
            paquete = bytes([gatillo_byte, joystick_byte])

            # 6. Enviar y mostrar
            if bt_sock:
                try:
                    bt_sock.send(paquete)
                except Exception as e:
                    print(f"\nError al enviar: {e}")
                    break
            
            # Limitar el print a consola a ~10 Hz (cada 10 ciclos) para no ralentizar el bucle
            loop_counter += 1
            if loop_counter >= 10:
                print(f"DEBUG -> G:{gatillo_byte:3} | E:{joystick_byte:3} (Hex: {paquete.hex()})      ", end='\r')
                loop_counter = 0

            time.sleep(SEND_INTERVAL)

    except KeyboardInterrupt:
        print("\n\nFinalizando programa...")
    finally:
        if bt_sock:
            bt_sock.close()
        pygame.quit()

if __name__ == "__main__":
    main()
