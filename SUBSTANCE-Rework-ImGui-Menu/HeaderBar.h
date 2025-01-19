#pragma once

#include "pch.h"

// ---------------------------------------------------------------------------------
// Encapsulamos nuestra clase HeaderBar en un namespace si lo deseas
// (Ajústalo según tu estructura).
//
class HeaderBar
{
public:
    // Identificador
    std::string ID = "header.default";

    // Propiedades principales
    ImVec2 Size = ImVec2(10.0f, 50.0f);
    ImVec4 BackgroundColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    // Propiedades del label izquierdo
    std::string LeftLabelText = "Izquierda";
    ImVec4 LeftLabelColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    float LeftLabelIndent = 15.0f;

    // Propiedades del label derecho
    std::string RightLabelText = "Derecha";
    ImVec4 RightLabelColor = ImVec4(0.580f, 0.580f, 0.596f, 1.000f);
    float RightLabelIndent = 15.0f;

    // Efecto de texto progresivo
    float TextRevealDelay = 0.2f; // Retraso en segundos por carácter
    double animationTimer = 0.0;
    int currentCharIndex = 0;

    // Generador de caracteres aleatorios (C++11).
    // En C++ real, puedes usar std::mt19937 con una semilla, etc.
    std::vector<char> randomChars = {
        '!','@','#','$','%','^','&','*','(',
        ')','-','_','=','+','[',']','{','}',
        '|',';',':','\'','\"',',','.',
        '<','>','?','/','\\','~','`'
    };

    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

    // Constructor: inicializar random
    HeaderBar();

    // Restablecer la animación
    void ResetAnimationTimer();

    // Método que actualiza y devuelve el texto animado
    std::string GetAnimatedText(double deltaTime);

    // Generar caracteres aleatorios para la parte oculta
    std::string GenerateRandomPart(int length);

    // Método para renderizar la barra
    void Render(double deltaTime);
};

//
// Ejemplo de uso en tu main code (pseudocódigo):
//
//  HeaderBar myHeader;
//  myHeader.Size = ImVec2(300, 50);
//  myHeader.LeftLabelText = "Left Animated";
//  myHeader.RightLabelText = "Right Static";
//
//  // Cada frame, llamas a:
//  double dt = ...; // deltaTime
//  myHeader.Render(dt);
//
