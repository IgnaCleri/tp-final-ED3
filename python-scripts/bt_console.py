import socket
import sys
import threading

def receive_data(sock):
    while True:
        try:
            data = sock.recv(1024)
            if not data:
                break
            print(f"\n[HC-05] < {data.hex()} (ASCII: {data.decode('ascii', errors='replace')})")
            print("> ", end="", flush=True)
        except:
            break

def main():
    MAC = "00:25:03:01:6F:C5"
    PORT = 1
    
    print(f"--- CONSOLA BLUETOOTH DIRECTA ---")
    print(f"Intentando conectar a {MAC}...")
    
    try:
        sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
        sock.connect((MAC, PORT))
        print("¡CONECTADO! Escribe algo y pulsa Enter para enviar.")
        print("Usa Ctrl+C para salir.\n")
        
        # Hilo para recibir datos en segundo plano
        threading.Thread(target=receive_data, args=(sock,), daemon=True).start()
        
        while True:
            text = input("> ")
            if text:
                sock.send(text.encode('utf-8'))
    except KeyboardInterrupt:
        print("\nCerrando consola...")
    except Exception as e:
        print(f"\nERROR: {e}")
    finally:
        if 'sock' in locals():
            sock.close()

if __name__ == "__main__":
    main()
