#pragma once

#include "components/simple_scene.h"
#include <vector>
#include "utils/glm_utils.h"
#include "lab_m1/tema1/text_renderer.h"


namespace transform2D
{
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::mat3(1, 0, 0, 0, 1, 0, translateX, translateY, 1);

    }

    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::mat3(scaleX, 0, 0, 0, scaleY, 0, 0, 0, 1);

    }


    inline glm::mat3 Rotate(float radians)
    {
        return glm::mat3(cos(radians), sin(radians), 0, -sin(radians), cos(radians), 0, 0, 0, 1);

    }
}   

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void DrawChicken(glm::vec2 position);


    public:
        glm::mat3 modelMatrix;
        glm::ivec2 resolution;
        float cellSize = 50;    
        bool showGrid = true;
        int randuri = 9;
        int coloane = 17;
        int rand_verzi = 1;
        int col_verzi = 10;
        int availablePieces = 10;   
        float cellSize_verde = 90;

        struct Piesa {
            glm::vec2 pozitie;            
            std::vector<std::pair<std::string, glm::vec2>> componente; 
            bool isSelected = false;
            std::vector<int> randuriOcupate;
            float scaraX = 1.05f;
            float scaraY = 1.05f;
            glm::vec2 offsetRelativ;

        };

        std::vector<Piesa> piese;
        bool dragging = false;
        Piesa* piesaSelectata = nullptr;
        glm::vec2 offsetDrag = glm::vec2(0, 0);
        Piesa piesaDrag;
        bool hasDragPiece = false;
        glm::vec2 dragStartPos = glm::vec2(0, 0); 

        glm::vec3 startButtonColor = glm::vec3(0, 1, 0); 
        bool gameStarted = false;
        bool navaBuna = false;

        int row_flacara_global;
        int row_flacara_extinsa;

        glm::vec4 verifPiese(const Piesa& p);
        int PieceType(const Piesa& p);

        struct PiesaSalvata {
            std::string tip;
            glm::vec2 pozitie;
        };
        std::vector<PiesaSalvata> navaSalvata;
        
        enum GameState { EDITOR, JOC, GAME_OVER, GAME_WIN, PAUZA };
        GameState gameState = EDITOR;
        std::vector<Piesa> navaInJoc;
        glm::vec2 navaPosition;    

        struct Proiectil {
            glm::vec2 position;
            float speed;
            bool active;
        };
        std::vector<Proiectil> proiectile;
        int scor = 0;
        glm::vec2 pozTun;
        float shipScale = 0.5f;  

        struct Inamic {
            glm::vec2 position;
            bool active;
            glm::vec2 startPosition;  
            float speed = 10.0f;    
            float amplitude = 50.0f;  
            float frequency = 2.0f;   
            float phase = 0.0f;   
        };

        std::vector<Inamic> inamici;
        float vitezaBaza = 100.0f; 
        float vitezaJucator = 0.0f;

        struct ProiectilInamic {
            glm::vec2 position;
            float speed;
            bool active;
            bool isGift = false;   
            bool bouncing = false;  
            glm::vec2 velocity = glm::vec2(0);
            float rotation = 0.0f;       
            int points = 0;
        };
        std::vector<ProiectilInamic> proiectileInamici;
        float timpTragere = 0.0f;    
        float intervalTragere = 2.0f; 

        int vieti = 4;
        glm::vec2 navaStartPosition = glm::vec2(610, 50); 

        gfxc::TextRenderer* textRenderer;

        int scorFinal = 0;
        bool toateInamiciiUcisi = false;
        int waveNumber = 1;             
        float waveTimer = 0.0f;    
        bool asteaptaUrmatorulVal = false;  

        struct ParticulaCopanel {
            glm::vec2 position;
            glm::vec2 velocity;
            float rotation;
            float angularSpeed;
            bool active;
        };
        std::vector<ParticulaCopanel> copanei;


        struct ParticulaExplozie {
            glm::vec2 position;
            glm::vec2 velocity;
            float lifetime;     
            float rotation;
            bool active;
        };
        std::vector<ParticulaExplozie> particuleExplozie;

        bool cameraShakeActive = false;
        float cameraShakeTime = 0.0f;
        float cameraShakeDuration = 0.3f; 
        float cameraShakeIntensity = 20.0f; 

        std::vector<glm::vec2> stars;
        float starSpeed = 70.0f;

        bool bouncing = false; 
        int bounceCount = 0;    


    private:
        bool VerificaNava();
        bool EsteConexa(const std::vector<std::vector<int>>& grid);
		void UpdateStartButton();
        void GenereazaInamici();
        void RenderEditorMode(float deltaTimeSeconds);
        void LoadFiguri();
        bool overlapFunc(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
    };

}   



