import pygame
import time
import sys

# --- CONFIGURACIÓN ---
SEND_INTERVAL = 0.01          # Intervalo de actualización en segundos (100Hz - Máxima velocidad recomendada)

def main():
    # 1. Inicializar Pygame y Joystick
    pygame.init()
    pygame.joystick.init()

    if pygame.joystick.get_count() == 0:
        print("ERROR: No se encontró ningún mando conectado. Conecta uno y vuelve a intentar.")
        return

    mando = pygame.joystick.Joystick(0)
    mando.init()
    print(f"--- MODO DEBUG CONSOLA ---")
    print(f"Mando detectado: {mando.get_name()}")
    print(f"Número de ejes: {mando.get_numaxes()}")
    print(f"Número de botones: {mando.get_numbuttons()}")
    print(f"Número de hats (D-pad): {mando.get_numhats()}")

    # --- ASIGNACIÓN DE EJES Y BOTONES ---
    INDEX_JOYSTICK = 0
    INDEX_GATILLO = 5
    
    # Índices típicos para PS4 en Linux/Pygame
    BTN_CROSS = 0
    BTN_CIRCLE = 1
    BTN_SQUARE = 2
    BTN_TRIANGLE = 3

    print(f"\nMostrando datos en tiempo real...")
    print("Mapa de bits Byte 3: [Der, Izq, Abajo, Arriba, Tri, Cua, Cir, Equis]")
    print("Presiona Ctrl+C para salir.\n")

    loop_counter = 0
    try:
        while True:
            pygame.event.pump()

            # 2. Leer valores de ejes
            val_joystick = mando.get_axis(INDEX_JOYSTICK)
            val_gatillo = mando.get_axis(INDEX_GATILLO)

            # 3. Leer valores de botones
            btn_cross = mando.get_button(BTN_CROSS)
            btn_circle = mando.get_button(BTN_CIRCLE)
            btn_square = mando.get_button(BTN_SQUARE)
            btn_triangle = mando.get_button(BTN_TRIANGLE)

            # 4. Leer D-pad (flechitas)
            hat = (0, 0)
            if mando.get_numhats() > 0:
                hat = mando.get_hat(0)
            
            dpad_up = 1 if hat[1] == 1 else 0
            dpad_down = 1 if hat[1] == -1 else 0
            dpad_left = 1 if hat[0] == -1 else 0
            dpad_right = 1 if hat[0] == 1 else 0

            # 5. Normalización (0-255)
            # Usamos round() en lugar de int() directo para evitar que 254.99 se convierta en 254
            gatillo_byte = int(round(((val_gatillo + 1) / 2) * 255))
            joystick_byte = int(round(((val_joystick + 1) / 2) * 255))

            gatillo_byte = max(0, min(255, gatillo_byte))
            joystick_byte = max(0, min(255, joystick_byte))

            # 6. Construir el tercer byte (Nuevo orden solicitado)
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

            # 7. Formatear salida para consola
            # Mostramos los bits en orden de lectura humana (Bit 7 a Bit 0)
            # [Izq, Arr, Der, Aba] [Cua, Tri, Cir, Equ]
            bits_str = f"{botones_byte:08b}"
            bits_formatted = f"{bits_str[0:4]} {bits_str[4:8]}"
            
            # Limitar el print a consola a ~10 Hz (cada 10 ciclos) para no ralentizar el bucle
            loop_counter += 1
            if loop_counter >= 10:
                print(f"Gatillo: {gatillo_byte:3} | Joystick: {joystick_byte:3} | Byte: {bits_formatted} (Hex: {botones_byte:02X})", end='\r')
                loop_counter = 0

            time.sleep(SEND_INTERVAL)

    except KeyboardInterrupt:
        print("\n\nSaliendo del modo debug...")
    finally:
        pygame.quit()

if __name__ == "__main__":
    main()
