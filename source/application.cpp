#include "application.h"
#include "helpers/imgui_sfml.h"
#include "common.h"
#include "track.h"
#include "windows.h"


#include <SFML/Window/Event.hpp>
#include <imgui.h>
#include <cstdlib>
#include <ctime>

Application::Application() :
    m_window(sf::VideoMode(1600, 900), "AI Cars", sf::Style::Close),
    m_dbgDrawHelper(m_window)
{
    m_window.setFramerateLimit(60);

    b2Vec2 gravity(0.f, 0.f);
    m_world = std::make_unique<b2World>(gravity);

    m_dbgDrawHelper.SetFlags(
        DebugDraw::e_shapeBit
        // | DebugDraw::e_jointBit
    );
    m_world->SetDebugDraw(&m_dbgDrawHelper);

    createTrack(*m_world.get());

	printf("Generating cars: ");
    for (int i = 0; i < carCount; i++)
    {
		m_car.push_back(std::make_unique<Car>(m_world.get()));
		//Sleep(100);
		m_car[i]->randomizeNetworkWeights();     
		//printf("%d ",i);
		
    }
	srand(time(NULL));
}

void Application::run()
{
    im_sf::init(m_window);

    sf::Clock clock;
    while (m_window.isOpen()) {
        processEvents();
        update(clock.restart());
        render();
    }

    im_sf::shutdown();
}

void Application::processEvents()
{
    sf::Event e;
    while (m_window.pollEvent(e))
    {
		if (e.type == sf::Event::Closed)
			m_window.close();

		else if (e.type == sf::Event::KeyPressed)
		{
			switch (e.key.code)
			{
			case sf::Keyboard::A:
				m_car[0]->rotation = -1;
				break;
			case sf::Keyboard::D:
				m_car[0]->rotation = 1;
				break;
			case sf::Keyboard::W:
				m_car[0]->speed = 1;
				break;
			case sf::Keyboard::S:
				m_car[0]->speed = -1;
				break;
			}
		}

		else if (e.type == sf::Event::KeyReleased)
		{
			switch (e.key.code)
			{
			case sf::Keyboard::A:
			case sf::Keyboard::D:
				m_car[0]->rotation = 0;
				break;
			case sf::Keyboard::W:
			case sf::Keyboard::S:
				m_car[0]->speed = 0;
				break;
			}				
		}		

        else if (e.type == sf::Event::KeyPressed)
        {
            switch (e.key.code)
            {
            case sf::Keyboard::A:
                m_car[0]->rotation = -1;
                break;
            case sf::Keyboard::D:
                m_car[0]->rotation = 1;
                break;
            case sf::Keyboard::W:
                m_car[0]->speed = 1;
                break;
            case sf::Keyboard::S:
                m_car[0]->speed = -1;
                break;
            }
        }

        else if (e.type == sf::Event::KeyReleased)
        {
            switch (e.key.code)
            {
            case sf::Keyboard::A:
            case sf::Keyboard::D:
                m_car[0]->rotation = 0;
                break;
            case sf::Keyboard::W:
            case sf::Keyboard::S:
                m_car[0]->speed = 0;
                break;
            }
        }
        im_sf::processEvents(e);
    }
}

void Application::update(const sf::Time& dt)
{
    for(unsigned int i = 0; i < m_car.size(); i ++) m_car[i]->update();

	if (Car::stoppedCars == m_car.size())
	{		
		int best_car_index, second_best_car_index;
		//std::unique_ptr<Car> best_car, second_best_car;
		double ***best_weights, ***second_best_weights;
		best_car_index = Car::carsWeightsSorted.size()-1;
		second_best_car_index = Car::carsWeightsSorted.size()-2;

		//find the fittest cars in current population
		/*for (unsigned int i = 0; i < m_car.size(); i++)
		{
			if (Car::carsFitness[i] > Car::carsFitness[best_car_index])
				best_car_index = i;
			else if (Car::carsFitness[i] > Car::carsFitness[second_best_car_index])
				second_best_car_index = i;
		}*/

		best_weights = Car::carsWeightsSorted[best_car_index];
		second_best_weights = Car::carsWeightsSorted[second_best_car_index];
		//best_weights = copy_weights_array(m_car[best_car_index]->getNetworkWeights(), 2);
		//second_best_weights = copy_weights_array(m_car[second_best_car_index]->getNetworkWeights(), 2);

		//deleting whole current population, it is not needed anymore
		m_car.clear();

		for (int i = 0; i < carCount/2 - 1; i++)
		{
			m_car.push_back(std::make_unique<Car>(m_world.get()));
			m_car.push_back(std::make_unique<Car>(m_world.get()));


			if ((float)rand() / RAND_MAX < crossover_chance)
			{
				//crossover between two best cars
				m_car[m_car.size()-1]->crossover(best_weights, second_best_weights);
				m_car[m_car.size()-2]->crossover(second_best_weights, best_weights);
			}
			else
			{
				//cloning best and second best car
				m_car[m_car.size()-1]->setNetworkWeights(best_weights);
				m_car[m_car.size()-2]->setNetworkWeights(second_best_weights);
			}		

			if ((float)rand() / RAND_MAX < mutation_chance)
			{
				m_car[m_car.size() - 1]->mutate();
				m_car[m_car.size() - 2]->mutate();
			}
		}
		
		//add two best cars to new population*/
		m_car.push_back(std::make_unique<Car>(m_world.get()));
		m_car.push_back(std::make_unique<Car>(m_world.get()));
		m_car[m_car.size() - 1]->setNetworkWeights(best_weights);
		m_car[m_car.size() - 2]->setNetworkWeights(second_best_weights);
		
		Car::carsWeightsSorted.clear();
		Car::stoppedCars = 0;
	}

    m_world->Step(1 / 60.f, 10, 8);
    im_sf::update(dt);
}

void Application::render()
{
    m_window.clear();

    m_world->DebugDraw();

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
    ImGui::SetNextWindowPos({ 0.f, 0.f });
    if (ImGui::Begin("Options", nullptr, flags)) {
        ImGui::SliderFloat(STRINGIFY(renderScaleFactor), &renderScaleFactor, 16.f, 64.f);
        if (ImGui::Button("Start Simulation")) {
            // Code for starting simulation
        }
    }
    ImGui::End();

    //temp
    //ImGui::ShowDemoWindow();

    im_sf::render();
    m_window.display();
}

double *** Application::copy_weights_array(double *** source, int layers_count, int* layers_neurons_count)
{
	double ***result = new double**[layers_count];
	for (int i = 0; i < layers_count; i++)
	{
		result[i] = source[i];
	}
	return result;

}
