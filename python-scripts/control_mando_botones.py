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

    # --- ASIGNACIÓN DE EJES Y BOTONES ---
    INDEX_JOYSTICK = 0
    INDEX_GATILLO = 5
    
    # Índices típicos para PS4 en Linux/Pygame
    # 0: Equis, 1: Círculo, 2: Cuadrado, 3: Triángulo
    BTN_CROSS = 0
    BTN_CIRCLE = 1
    BTN_SQUARE = 2
    BTN_TRIANGLE = 3

    print(f"\nTransmitiendo datos (Gatillo Eje {INDEX_GATILLO}, Joystick Eje {INDEX_JOYSTICK}, Botones y D-pad)...")
    print("Presiona Ctrl+C para salir.\n")

    loop_counter = 0
    try:
        while True:
            pygame.event.pump()

            # 3. Leer valores de ejes
            val_joystick = mando.get_axis(INDEX_JOYSTICK)
            val_gatillo = mando.get_axis(INDEX_GATILLO)

            # 4. Leer valores de botones
            btn_cross = mando.get_button(BTN_CROSS)
            btn_circle = mando.get_button(BTN_CIRCLE)
            btn_square = mando.get_button(BTN_SQUARE)
            btn_triangle = mando.get_button(BTN_TRIANGLE)

            # 5. Leer D-pad (flechitas)
            # mando.get_hat(0) devuelve (x, y) donde x,y son -1, 0, o 1
            hat = (0, 0)
            if mando.get_numhats() > 0:
                hat = mando.get_hat(0)
            
            dpad_up = 1 if hat[1] == 1 else 0
            dpad_down = 1 if hat[1] == -1 else 0
            dpad_left = 1 if hat[0] == -1 else 0
            dpad_right = 1 if hat[0] == 1 else 0

            # 6. Normalización para protocolo de 8 bits (0-255)
            gatillo_byte = int(round(((val_gatillo + 1) / 2) * 255))
            joystick_byte = int(round(((val_joystick + 1) / 2) * 255))

            gatillo_byte = max(0, min(255, gatillo_byte))
            joystick_byte = max(0, min(255, joystick_byte))

            # 7. Construir el tercer byte (Botones y D-pad)
            # Mapa de bits: 
            # Bits 0-3: Equis, Círculo, Triángulo, Cuadrado
            # Bits 4-7: Abajo, Derecha, Arriba, Izquierda
            botones_byte = (btn_cross << 0) | \
                           (btn_circle << 1) | \
                           (btn_triangle << 2) | \
                           (btn_square << 3) | \
                           (dpad_down << 4) | \
                           (dpad_right << 5) | \
                           (dpad_up << 6) | \
                           (dpad_left << 7)

            # 8. Crear paquete binario (3 bytes)
            paquete = bytes([gatillo_byte, joystick_byte, botones_byte])

            # 9. Enviar y mostrar
            if bt_sock:
                try:
                    bt_sock.send(paquete)
                except Exception as e:
                    print(f"\nError al enviar: {e}")
                    break
            
            # Limitar el print a consola a ~10 Hz (cada 10 ciclos) para no ralentizar el bucle
            loop_counter += 1
            if loop_counter >= 10:
                print(f"DEBUG -> G:{gatillo_byte:3} | J:{joystick_byte:3} | B:{botones_byte:08b} (Hex: {paquete.hex()})      ", end='\r')
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
