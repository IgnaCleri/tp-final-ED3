### Refactorizacion de control de servo con bluetooth

Bue refactorize todo y cambie la implementacion por uart0 a uart2 por que nacho me lo pidio


### Asegura que la carpeta (modules/src)no esté excluida del Build (para los archivos .c)

- Haz clic derecho sobre la carpeta src que esta en modules y selecciona Properties.

- En el menú de la izquierda, ve a C/C++ Build y asegúrate de que la casilla que dice Exclude from build (Excluir de la compilación) esté desmarcada. 

- Si está marcada, el compilador ignorará por completo los archivos .c dentro de ella.

**Nota1:** incluye la carpeta inc de modules con los .h para los includes del archivo principal
**Nota2:** Obvio no lo probe pero deberia andar exactamente igual, en mi proyecto compila asi que supongo que funcionara igual
por las dudas dejo un zip con lo que estaba antes dentro de la carpeta (aun usando UART0)
