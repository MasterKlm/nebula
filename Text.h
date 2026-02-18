#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>




class Text 
{

    public: 
        SDL_Texture* texture = nullptr;
        TTF_Font* font = nullptr;
        SDL_Renderer* renderer = nullptr;
        int x;
        int y;
        SDL_Rect dstRect;
        Text(const char* fontPath, std::string text, int size, SDL_Color color, int x, int y, SDL_Renderer* rend)
        {
            this->x = x;
            this->y = y;
            this->renderer = rend;
            
            font = TTF_OpenFont(fontPath, size);
            if(!font){
                std::cout << "Error loading font" << "\n";
                return;
            }

            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
            if(!surface){
                std::cout << "Error creating text surface" << "\n";
                TTF_CloseFont(font);
                return;
            }

            texture = SDL_CreateTextureFromSurface(renderer, surface);
            dstRect = {x, y, surface->w, surface->h};
            SDL_FreeSurface(surface);

            

        }
        void render()
        {
            if (!texture || !renderer) return;
            SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        }
        void setText(const std::string &text, SDL_Color color)
        {
            if (!font || !renderer) return;
            if(texture) { SDL_DestroyTexture(texture); texture = nullptr; }

            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
            if(!surface){
                std::cout << "Error creating text surface" << "\n";
                return;
            }

            texture = SDL_CreateTextureFromSurface(renderer, surface);
            dstRect = {x, y, surface->w, surface->h};
            SDL_FreeSurface(surface);
        }
        ~Text()
        {
            if(texture) SDL_DestroyTexture(texture);
            if(font) TTF_CloseFont(font);
        }
        
};







#endif