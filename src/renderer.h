#pragma once

#include <vector>
#include <memory>
#include <thread>

#include <SDL2/SDL.h>

#include "scene.h"

class Renderer : public boost::noncopyable {
	public:
		Renderer(const Scene& scene, SDL_Window* window, SDL_Renderer* renderer);
		~Renderer();

		void setCamera(Camera& cam);

		void resize(std::size_t /*w*/, std::size_t /*h*/);

		SDL_Texture* texture();
		int currentTexture() const;

	private:
		void startRenderThread();

		void renderAll();
		void renderFrame();

		void initTextures();

		const Scene* m_scene;
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;

		std::vector<SDL_Texture*> m_textures;
		int m_currentTexture;

		Camera m_camera;

		bool m_rendering;
		std::unique_ptr<std::thread> m_thread;
};
