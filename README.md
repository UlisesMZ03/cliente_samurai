### 🥷 Cliente Samurai - Simulación Cliente-Servidor con Búsqueda de Caminos Óptimos

<p align="center">
  <img width="300" src="https://i.imgur.com/K89m3DK.png" alt="Samurai Client">
</p>

**Cliente Samurai es una aplicación de escritorio desarrollada en C++ con Qt**, que forma parte de un sistema **cliente-servidor** para simular el movimiento de un samurái a través de un entorno con obstáculos.  
El cliente se conecta al servidor para recibir información del mapa y enviar movimientos óptimos utilizando algoritmos como **A*** y **BFS**.

---

### 🚀 ¿Cómo compilar?

```bash
# Clonar el repositorio
git clone https://github.com/UlisesMZ03/cliente_samurai.git

# Abrir el archivo .pro en Qt Creator
# Compilar y ejecutar desde el IDE
```

> Requiere Qt 5 o superior y un entorno con compilador C++

---

### 🤖 Algoritmos implementados

- 🔍 A* (A-Star)
- 🔄 BFS (Breadth-First Search)
- 🧠 Preparado para integrar otras estrategias de búsqueda

---

### 🌐 Arquitectura Cliente-Servidor

- 🖥️ El cliente envía comandos y peticiones de ruta
- 🧭 El servidor gestiona el entorno, obstáculos y lógica de juego
- 🔌 Comunicación basada en sockets o protocolos simples

---

### 🎯 Objetivo del proyecto

Simular la toma de decisiones inteligentes en un juego mediante algoritmos de búsqueda, en una arquitectura distribuida. Ideal como base para proyectos de IA, sistemas distribuidos o videojuegos.

---

### 📂 Estructura

- `main.cpp`: Punto de entrada
- `mainwindow.cpp / .h`: Ventana principal y lógica del cliente
- `untitled4.pro`: Configuración para Qt Creator

---

### 📚 Recursos útiles

- [🧭 Visualización A* Pathfinding](https://www.redblobgames.com/pathfinding/a-star/introduction.html)
- [📘 Qt Official Docs](https://doc.qt.io/)
- [🔧 Sockets en C++ con Qt](https://doc.qt.io/qt-5/qtnetwork-index.html)
