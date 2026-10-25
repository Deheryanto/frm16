#include <SDL2/SDL.h>
#include <iostream>
#include "memory/VRAM.h"
#include "cpu/CPU.h"
#include "io/IO.h"

using namespace frm;

// --- Display & Rendering Scaling Constants ---
constexpr int SCALE    = 4;
constexpr int WINDOW_W = 128 * SCALE;
constexpr int WINDOW_H = 128 * SCALE;

/// @brief Runs the main application loop, handling SDL2 initialization, input events, CPU steps, and VRAM rendering.
/// @param vram Reference to the system VRAM instance.
/// @param cpu Reference to the CPU execution core.
/// @param io Reference to the I/O manager.
void run_display(VRAM& vram, CPU& cpu, IO& io) {
      // Initialize SDL2 video subsystem
      if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
            return;
      }

      // Create SDL window and accelerated renderer with VSync enabled
      SDL_Window* window = SDL_CreateWindow(
            "FRM16 VM Display", 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            WINDOW_W, 
            WINDOW_H, 
            SDL_WINDOW_SHOWN
      );
      
      SDL_Renderer* renderer = SDL_CreateRenderer(
            window, 
            -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
      );
      
      SDL_Texture* texture = SDL_CreateTexture(
            renderer, 
            SDL_PIXELFORMAT_RGB24, 
            SDL_TEXTUREACCESS_STREAMING, 
            128, 
            128
      );

      // Pixel buffer mapping RGB24 color values for the 128x128 frame buffer
      uint8_t pixels[128 * 128 * 3]; 
      
      bool running = true;
      SDL_Event event;

      // Configure hardware timer and interrupt behavior on startup
      cpu.enableInterrupt(true);
      cpu.setTimerReload(500);
      cpu.setInterruptVector(0x0004);

      while (running) {
            // --- 1. Event Handling (Keyboard Input & Window Close) ---
            while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) {
                        running = false;
                  } else if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                              case SDLK_UP:    
                              case SDLK_w: io.setButton(IO_BUTTON_UP, true);    break;
                              case SDLK_DOWN:  
                              case SDLK_s: io.setButton(IO_BUTTON_DOWN, true);  break;
                              case SDLK_LEFT:  
                              case SDLK_a: io.setButton(IO_BUTTON_LEFT, true);  break;
                              case SDLK_RIGHT: 
                              case SDLK_d: io.setButton(IO_BUTTON_RIGHT, true); break;
                        }
                  } else if (event.type == SDL_KEYUP) {
                        switch (event.key.keysym.sym) {
                              case SDLK_UP:    
                              case SDLK_w: io.setButton(IO_BUTTON_UP, false);    break;
                              case SDLK_DOWN:  
                              case SDLK_s: io.setButton(IO_BUTTON_DOWN, false);  break;
                              case SDLK_LEFT:  
                              case SDLK_a: io.setButton(IO_BUTTON_LEFT, false);  break;
                              case SDLK_RIGHT: 
                              case SDLK_d: io.setButton(IO_BUTTON_RIGHT, false); break;
                        }
                  }
            } 

            // --- 2. CPU Execution Step ---
            if (!cpu.isHalted()) {
                  // Execute multiple instructions per frame for smoother performance
                  for (int i = 0; i < 10; ++i) {
                        cpu.step();
                        if (cpu.isHalted()) break;
                  }
            }

            // --- 3. VRAM Rendering & Color Palette Mapping ---
            for (int y = 0; y < 128; ++y) {
                  for (int x = 0; x < 128; ++x) {
                        uint8_t val = vram.read(y * 128 + x);
                        
                        uint8_t r = 0, g = 0, b = 0;
                        switch (val) {
                              case 1:  r = 255; g = 255; b = 255; break; // White
                              case 2:  r = 255; g = 0;   b = 0;   break; // Red
                              case 3:  r = 0;   g = 255; b = 0;   break; // Green
                              case 4:  r = 0;   g = 0;   b = 255; break; // Blue
                              case 5:  r = 255; g = 255; b = 0;   break; // Yellow
                              case 6:  r = 255; g = 0;   b = 255; break; // Magenta
                              case 7:  r = 0;   g = 255; b = 255; break; // Cyan
                              default: r = 0;   g = 0;   b = 0;   break; // Black (default)
                        }
                        
                        int idx = (y * 128 + x) * 3;
                        pixels[idx + 0] = r;
                        pixels[idx + 1] = g;
                        pixels[idx + 2] = b;
                  }
            }

            // Update SDL texture and render frame
            SDL_UpdateTexture(texture, nullptr, pixels, 128 * 3);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
      }

      // --- Cleanup SDL Resources ---
      SDL_DestroyTexture(texture);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      SDL_Quit();
}