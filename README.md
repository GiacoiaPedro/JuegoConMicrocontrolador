# 🔐 Recordar la Contraseña - Juego Interactivo con Atmega328P

Este proyecto implementa un juego interactivo llamado **"Recordar la Contraseña"**, diseñado para funcionar sobre un microcontrolador **Atmega328p**, utilizando un **display LCD 2x16**, un **teclado matricial 4x4**, e interrupciones de **Timer**. El objetivo del juego es memorizar y luego ingresar correctamente una palabra aleatoria de 5 caracteres seleccionada desde un diccionario.

---

## 🎯 Objetivo del Juego

1. Al presionar la tecla `*`, el sistema selecciona una palabra aleatoria del diccionario.
2. La palabra se muestra durante 2 segundos en el LCD.
3. Luego de desaparecer, el jugador debe ingresar la palabra carácter por carácter usando el teclado.
4. Cada letra ingresada se confirma con la tecla `#`.
5. Si se ingresa correctamente la palabra:
   - Se muestra un mensaje de **"victoria"** junto con el tiempo que tomó completarla.
6. Si se cometen **3 errores**, se muestra un mensaje de **"derrota"** y se reinicia el juego tras 5 segundos.

---

## 📦 Estructura del Proyecto

El proyecto está modularizado por archivos:

- `fsm.c / fsm.h` – Lógica principal del juego con una Máquina de Estados Finita (MEF).
- `dict.c / dict.h` – Diccionario de palabras y funciones de selección aleatoria.
- `lcd.c / lcd.h` – Control del display LCD.
- `keypad.c / keypad.h` – Lectura de teclas del teclado matricial 4x4.
- `timer.c / timer.h` – Configuración e interrupción del Timer para temporización.
- `main.c` – Inicialización general y bucle principal.

---

## ⚙️ Requisitos Técnicos

- Microcontrolador: **Atmega328P**
- Display: **LCD 2x16 (HD44780 compatible)**
- Entrada: **Teclado matricial 4x4**
- Compilador: Compatible con **AVR-GCC**
- IDE sugerido: **Microchip Studio**
- Simulador recomendado: **Proteus**

---

## 🧠 Lógica del Juego (MEF)

- **ST_IDLE**: Espera la tecla `*` para comenzar.
- **ST_SHOW_WORD**: Muestra una palabra aleatoria durante 2 segundos.
- **ST_TYPING**: El jugador debe ingresar la palabra, confirmando cada letra con `#`.
- **ST_CHECK**: Verifica si el carácter ingresado es correcto.
- **ST_VICTORY / ST_DEFEAT**: Muestra resultado final y espera 5 segundos antes de reiniciar.

---

## 📚 Diccionario de Palabras

Contiene palabras de 5 letras, incluyendo mayúsculas, minúsculas y combinaciones:

Arbol, Boton, CDyMC, ClavE, Facil, Gafas, Hojas, LiBro, Lanza,
Nieve, PeRro, PecES, PiAno, PrYKe, RUEDa, SERIE, SalUd, Salud,
Silla, Tecla, Valor, Verde, YnHRz, hARdD, silla


---

## 🧪 Simulación en Proteus

Se puede simular el juego usando Proteus con:
- Atmega328p.
- Módulo LCD 2x16 conectado al puerto correspondiente.
- Teclado 4x4 conectado a los pines configurados.
- Oscilador a 16 MHz (recomendado).

---

## 🔁 Temporización

- La MEF se ejecuta mediante una **interrupción periódica de Timer**, no con retardo activo.
- Los tiempos (2s y 5s) se gestionan con una variable de milisegundos (`ticksMS`) incrementada en la ISR del Timer.

---

## ✅ Características Adicionales

- Manejo de errores (máximo 3 fallos).
- Rechazo de caracteres no alfabéticos (contabilizados como error).
- Randomización basada en `ticksMS` para obtener una palabra distinta en cada juego.
- Pantallas adaptadas a cada estado del juego.
