
#include "lab_m1/tema1/tema1.h"
#include "lab_m1/tema1/text_renderer.h"
#include <vector>
#include <iostream>


using namespace std;
using namespace m1;


Tema1::Tema1()
{
} 


Tema1::~Tema1()
{
}

glm::vec3 startButtonColor = glm::vec3(0, 1, 0);


void Tema1::Init()
{
    gameState = EDITOR;
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    
    this->resolution = window->GetResolution();
    navaPosition = navaStartPosition;

    glm::ivec2 res = window->GetResolution();
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, res.x, res.y);
    std::string fontPath = PATH_JOIN(window->props.selfDir, "src","lab_m1", "tema1", "Hack-Bold.ttf");
    textRenderer->Load(fontPath, 60);

	LoadFiguri();
}

void m1::Tema1::LoadFiguri() {

    //creez mesh patrat pu grid
    std::vector<VertexFormat> v =
    {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.32f, 0.55f, 0.95f)),
        VertexFormat(glm::vec3(cellSize, 0, 0), glm::vec3(0.32f, 0.55f, 0.95f)),
        VertexFormat(glm::vec3(cellSize, cellSize, 0), glm::vec3(0.32f, 0.55f, 0.95f)),
        VertexFormat(glm::vec3(0, cellSize, 0), glm::vec3(0.32f, 0.55f, 0.95f))
    };
    std::vector<unsigned int> idx = { 0, 1, 2, 0, 2, 3 };
    Mesh* square = new Mesh("square");
    square->InitFromData(v, idx);
    AddMeshToList(square);

    // liniile albastre pentru chenar la grid
    std::vector<VertexFormat> lineVerts = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.4f, 1.0f)),
        VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.0f, 0.4f, 1.0f))
    };
    std::vector<unsigned int> lineIdx = { 0, 1 };
    Mesh* line = new Mesh("line");
    line->SetDrawMode(GL_LINES);
    line->InitFromData(lineVerts, lineIdx);
    AddMeshToList(line);

    // liinile rosii pe ecran
    std::vector<VertexFormat> lineVerts_red = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0))
    };
    std::vector<unsigned int> lineIdx_red = { 0, 1 };

    Mesh* line_red = new Mesh("red_line");
    line_red->SetDrawMode(GL_LINES);
    line_red->InitFromData(lineVerts_red, lineIdx_red);
    AddMeshToList(line_red);

    // patratele verzi de sus(nr piese)
    std::vector<VertexFormat> v_green = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(cellSize_verde, 0, 0), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(cellSize_verde, cellSize_verde, 0), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(0, cellSize_verde, 0), glm::vec3(0, 1, 0))
    };
    std::vector<unsigned int> idx_green = { 0, 1, 2, 0, 2, 3 };
    Mesh* green_square = new Mesh("green_square");
    green_square->InitFromData(v_green, idx_green);
    AddMeshToList(green_square);

    //buton start joc 
    float c = cellSize_verde;
    std::vector<VertexFormat> verts = {
        VertexFormat(glm::vec3(c, c, 0), startButtonColor),
        VertexFormat(glm::vec3(0, c, 0), startButtonColor),
        VertexFormat(glm::vec3(0, 0, 0), startButtonColor),
        VertexFormat(glm::vec3(c, 0, 0), startButtonColor),
        VertexFormat(glm::vec3(c / 2, c / 2, 0), startButtonColor)
    };
    std::vector<unsigned int> idx_fig = { 0, 1, 4, 1, 2, 4, 2, 3, 4 };
    Mesh* arrow_square = new Mesh("arrow_square");
    arrow_square->InitFromData(verts, idx_fig);
    AddMeshToList(arrow_square);

    // tija tun
    float dim = 50.0f;
    float laturaPatrat = dim;
    float grosime = laturaPatrat - 10;
    float inaltime_t = 3 * laturaPatrat;
    std::vector<VertexFormat> tijaVerts = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.3f, 0.3f, 0.3f)),
        VertexFormat(glm::vec3(grosime, 0, 0), glm::vec3(0.3f, 0.3f, 0.3f)),
        VertexFormat(glm::vec3(grosime, inaltime_t, 0), glm::vec3(0.3f, 0.3f, 0.3f)),
        VertexFormat(glm::vec3(0, inaltime_t, 0), glm::vec3(0.3f, 0.3f, 0.3f))
    };
    std::vector<unsigned int> tijaIdx = { 0, 1, 2, 0, 2, 3 };
    Mesh* tijaMesh = new Mesh("tija");
    tijaMesh->InitFromData(tijaVerts, tijaIdx);
    AddMeshToList(tijaMesh);

    // semicerc baza tun
    std::vector<VertexFormat> semicercVerts_tun;
    std::vector<unsigned int> semicercIdx_tun;
    float raza_tun = 25;
    int numSegments_tun = 20;
    glm::vec3 colorCap_tun = glm::vec3(0.95f, 0.95f, 0.7f);

    float dim_tun = 30.0f;
    semicercVerts_tun.push_back(VertexFormat(glm::vec3(dim_tun, dim, 0), colorCap_tun)); // centru
    // gener punctele de pe arc (stanga->dreapta)
    for (int i = 0; i <= numSegments_tun; i++) {
        float angle_tun = glm::pi<float>() * i / numSegments_tun;
        float x_tun = dim_tun + raza_tun * cos(angle_tun);
        float y_tun = dim + raza_tun * sin(angle_tun);
        semicercVerts_tun.push_back(VertexFormat(glm::vec3(x_tun, y_tun, 0), colorCap_tun));
    }
    // creez triunghiurile( pornesc din centru)
    for (int i = 1; i <= numSegments_tun; i++) {
        semicercIdx_tun.push_back(0);
        semicercIdx_tun.push_back(i);
        semicercIdx_tun.push_back(i + 1);
    }
    Mesh* semicerc_tun = new Mesh("semicerc_tun");
    semicerc_tun->InitFromData(semicercVerts_tun, semicercIdx_tun);
    AddMeshToList(semicerc_tun);

    //dreptunghi tun baza jos
    std::vector<VertexFormat> dreptunghiVerts_gri = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5)),
        VertexFormat(glm::vec3(dim, 0, 0), glm::vec3(0.5, 0.5, 0.5)),
        VertexFormat(glm::vec3(dim, dim, 0), glm::vec3(0.5, 0.5, 0.5)),
        VertexFormat(glm::vec3(0, dim, 0), glm::vec3(0.5, 0.5, 0.5))
    };
    std::vector<unsigned int> dreptunghiIdx_gri = { 0, 1, 2, 0, 2, 3 };

    Mesh* dreptunghi_gri = new Mesh("dreptunghi_gri");
    dreptunghi_gri->InitFromData(dreptunghiVerts_gri, dreptunghiIdx_gri);
    AddMeshToList(dreptunghi_gri);

    // linie varf tun
    float laturaPatr = dim;
    float grosimeLinie = laturaPatr / 10.0f;
    float inaltimeLinie = laturaPatr;
    std::vector<VertexFormat> linieVerts = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.3f, 0.3f, 0.1f)),
        VertexFormat(glm::vec3(inaltimeLinie, 0, 0), glm::vec3(0.3f, 0.3f, 0.1f)),
        VertexFormat(glm::vec3(inaltimeLinie, grosimeLinie, 0), glm::vec3(0.3f, 0.3f, 0.1f)),
        VertexFormat(glm::vec3(0, grosimeLinie, 0), glm::vec3(0.3f, 0.3f, 0.1f))
    };
    std::vector<unsigned int> linieIdx = { 0, 1, 2, 0, 2, 3 };
    Mesh* linieMesh = new Mesh("linie");
    linieMesh->InitFromData(linieVerts, linieIdx);
    AddMeshToList(linieMesh);

    // motor bloc sus 
    std::vector<VertexFormat> baseVerts_oranj = {
        VertexFormat(glm::vec3(0, -20, 0), glm::vec3(1.0f, 0.5f, 0.0f)),
        VertexFormat(glm::vec3(dim, -20, 0), glm::vec3(1.0f, 0.5f, 0.0f)),
        VertexFormat(glm::vec3(dim, 60, 0), glm::vec3(1.0f, 0.5f, 0.0f)),
        VertexFormat(glm::vec3(0, 60, 0), glm::vec3(1.0f, 0.5f, 0.0f))
    };
    std::vector<unsigned int> baseIdx_oranj = { 0, 1, 2, 0, 2, 3 };
    Mesh* patrat_oranj = new Mesh("patrat_oranj");
    patrat_oranj->InitFromData(baseVerts_oranj, baseIdx_oranj);
    AddMeshToList(patrat_oranj);

    // baza jos motor(deasupra dinte)
    float dim_2 = 30.0f;
    std::vector<VertexFormat> dreptunghiVerts = {
        VertexFormat(glm::vec3(0, dim_2, 0), glm::vec3(1.0f, 0.7f, 0.1f)),
        VertexFormat(glm::vec3(dim, dim_2, 0), glm::vec3(1.0f, 0.7f, 0.1f)),
        VertexFormat(glm::vec3(dim, dim, 0), glm::vec3(1.0f, 0.7f, 0.1f)),
        VertexFormat(glm::vec3(0, dim, 0), glm::vec3(1.0f, 0.7f, 0.1f))
    };
    std::vector<unsigned int> dreptunghiIdx = { 0, 1, 2, 0, 2, 3 };

    Mesh* dreptunghi_flacara = new Mesh("dreptunghi_flacara");
    dreptunghi_flacara->InitFromData(dreptunghiVerts, dreptunghiIdx);
    AddMeshToList(dreptunghi_flacara);

    // flacara-dintii
    std::vector<VertexFormat> dintiVerts;
    std::vector<unsigned int> dintiIdx;
    glm::vec3 colorFlame = glm::vec3(1.0f, 0.7f, 0.1f);
    float latimeDinte = 17.0f;
    float inaltime = 40.0f;

    dintiVerts.push_back(VertexFormat(glm::vec3(0, dim_2, 0), colorFlame));
    dintiVerts.push_back(VertexFormat(glm::vec3(latimeDinte, dim_2, 0), colorFlame));
    dintiVerts.push_back(VertexFormat(glm::vec3(2 * latimeDinte, dim_2, 0), colorFlame));
    dintiVerts.push_back(VertexFormat(glm::vec3(3 * latimeDinte, dim_2, 0), colorFlame));
    //dinti varf jos
    dintiVerts.push_back(VertexFormat(glm::vec3(latimeDinte / 2, 0, 0), colorFlame));
    dintiVerts.push_back(VertexFormat(glm::vec3(1.5f * latimeDinte, 0, 0), colorFlame));
    dintiVerts.push_back(VertexFormat(glm::vec3(2.5f * latimeDinte, 0, 0), colorFlame));

    // fiecare dinte- 2 triunghiuri
    dintiIdx.insert(dintiIdx.end(), { 0, 1, 4 });
    dintiIdx.insert(dintiIdx.end(), { 1, 2, 5 });
    dintiIdx.insert(dintiIdx.end(), { 2, 3, 6 });
    Mesh* dinti_flacara = new Mesh("dinti_flacara");
    dinti_flacara->InitFromData(dintiVerts, dintiIdx);
    AddMeshToList(dinti_flacara);


    // flacara 
    std::vector<VertexFormat> flameVerts = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0.5, 0)),
        VertexFormat(glm::vec3(dim_2, 0, 0), glm::vec3(1, 0.5, 0)),
        VertexFormat(glm::vec3(20, 35, 0), glm::vec3(1, 0.5, 0))
    };
    std::vector<unsigned int> flameIdx = { 0, 1, 2 };
    Mesh* mesh_flacara = new Mesh("flacara");
    mesh_flacara->InitFromData(flameVerts, flameIdx);
    AddMeshToList(mesh_flacara);

    // bloc gri 1
    std::vector<VertexFormat> baseVerts = {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5)),
        VertexFormat(glm::vec3(dim, 0, 0), glm::vec3(0.5, 0.5, 0.5)),
        VertexFormat(glm::vec3(dim, dim, 0), glm::vec3(0.5, 0.5, 0.5)),
        VertexFormat(glm::vec3(0, dim, 0), glm::vec3(0.5, 0.5, 0.5))
    };
    std::vector<unsigned int> baseIdx = { 0, 1, 2, 0, 2, 3 };
    Mesh* patrat_gri = new Mesh("patrat_gri");
    patrat_gri->InitFromData(baseVerts, baseIdx);
    AddMeshToList(patrat_gri);


    {
        // baza ciuperca
        std::vector<VertexFormat> baseVerts_ciup = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f)),
            VertexFormat(glm::vec3(dim, 0, 0), glm::vec3(0.8f, 0.8f, 0.8f)),
            VertexFormat(glm::vec3(dim, dim, 0), glm::vec3(0.8f, 0.8f, 0.8f)),
            VertexFormat(glm::vec3(0, dim, 0), glm::vec3(0.8f, 0.8f, 0.8f))
        };
        std::vector<unsigned int> baseIdx_ciup = { 0, 1, 2, 0, 2, 3 };
        Mesh* patrat_ciup = new Mesh("patrat_ciuperca");
        patrat_ciup->InitFromData(baseVerts_ciup, baseIdx_ciup);
        AddMeshToList(patrat_ciup);

        // caciula ciuperca
        std::vector<VertexFormat> semicercVerts;
        std::vector<unsigned int> semicercIdx;
        float raza = 70;
        int numSegments = 60;
        glm::vec3 colorCap = glm::vec3(0.95f, 0.95f, 0.7f);
        semicercVerts.push_back(VertexFormat(glm::vec3(dim_2, dim, 0), colorCap));
        for (int i = 0; i <= numSegments; i++) {
            float angle = glm::pi<float>() * i / numSegments;
            float x = dim_2 + raza * cos(angle);
            float y = dim + (raza * 0.7f) * sin(angle);
            semicercVerts.push_back(VertexFormat(glm::vec3(x, y, 0), colorCap));
        }
        for (int i = 1; i <= numSegments; i++) {
            semicercIdx.push_back(0);
            semicercIdx.push_back(i);
            semicercIdx.push_back(i + 1);
        }
        Mesh* semicerc = new Mesh("semicerc");
        semicerc->InitFromData(semicercVerts, semicercIdx);
        AddMeshToList(semicerc);
    }

    //COMPONENTE PIESE FINALE
    const float logicalWidth = 1280.0f;
    const float logicalHeight = 720.0f;
    float scaleX = resolution.x / logicalWidth;
    float scaleY = resolution.y / logicalHeight;

    //  CIUPERCA 
    Piesa ciuperca;
    ciuperca.pozitie = glm::vec2(125 * scaleX, 580 * scaleY);
    ciuperca.componente = {
        {"semicerc", glm::vec2(-5, 0)},
        {"patrat_ciuperca", glm::vec2(0, 0)}
    };
    piese.push_back(ciuperca);

    //  TIJA 
    Piesa tija;
    tija.pozitie = glm::vec2(130 * scaleX, 375 * scaleY);
    tija.componente = {
        {"tija", glm::vec2(5 * scaleX, 0)},
        {"linie", glm::vec2(0, 147 * scaleY)},
        {"dreptunghi_gri", glm::vec2(0, 0)},
        {"semicerc_tun", glm::vec2(-5, -10)}

    };
    piese.push_back(tija);

	//  MOTOR
    Piesa flacara;
    flacara.pozitie = glm::vec2(130 * scaleX, 250 * scaleY);
    flacara.componente = {
        {"patrat_oranj", glm::vec2(0, 0)},
        {"dreptunghi_flacara", glm::vec2(0, -50)},
        {"dinti_flacara", glm::vec2(-0.5f, -50)}
    };
    piese.push_back(flacara);

    // BAZA
    Piesa baza;
    baza.pozitie = glm::vec2(130 * scaleX, 70 * scaleY);
    baza.componente = { {"patrat_gri", glm::vec2(0, 0)} };
    piese.push_back(baza);


    // GAINA(inamicul) 

    // aripa
    {
        std::vector<VertexFormat> verts = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.6f, 0.6f, 0.6f)), 
            VertexFormat(glm::vec3(40, 0, 0), glm::vec3(0.6f, 0.6f, 0.6f)),
            VertexFormat(glm::vec3(0, 40, 0), glm::vec3(0.6f, 0.6f, 0.6f))
        };
        std::vector<unsigned int> idx = { 0, 1, 2 };
        Mesh* wing_left = new Mesh("wing_left");
        wing_left->InitFromData(verts, idx);
        AddMeshToList(wing_left);
    }

    // corp
    {
        std::vector<VertexFormat> verts = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.2f, 0.6f, 1.0f)),   
            VertexFormat(glm::vec3(40, 0, 0), glm::vec3(0.2f, 0.6f, 1.0f)),
            VertexFormat(glm::vec3(0, 40, 0), glm::vec3(0.2f, 0.6f, 1.0f)),
            VertexFormat(glm::vec3(40, 40, 0), glm::vec3(0.2f, 0.6f, 1.0f))
        };
        std::vector<unsigned int> idx = { 0, 1, 2, 1, 3, 2 };
        Mesh* body = new Mesh("body");
        body->InitFromData(verts, idx);
        AddMeshToList(body);
    }

    // cap
    {
        std::vector<VertexFormat> verts;
        std::vector<unsigned int> idx;
        int segments = 30;
        float radius = 10;
        glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.8f); 
        verts.push_back(VertexFormat(glm::vec3(50, 30, 0), color)); 
        for (int i = 0; i <= segments; i++) {
            float angle = i * 2 * M_PI / segments;
            verts.push_back(VertexFormat(glm::vec3(50 + radius * cos(angle), 30 + radius * sin(angle), 0), color));
            if (i > 0)
                idx.insert(idx.end(), {
                    (unsigned int)0,
                    (unsigned int)i,
                    (unsigned int)(i + 1)
                    });
        }
        Mesh* head = new Mesh("head");
        head->InitFromData(verts, idx);
        AddMeshToList(head);
    }

    // ochi
    {
        std::vector<VertexFormat> verts;
        std::vector<unsigned int> idx;
        int segments = 20;
        float radius = 2.5f;
        glm::vec3 color = glm::vec3(0, 0, 0);
        verts.push_back(VertexFormat(glm::vec3(55, 33, 0), color));
        for (int i = 0; i <= segments; i++) {
            float angle = i * 2 * M_PI / segments;
            verts.push_back(VertexFormat(glm::vec3(55 + radius * cos(angle), 33 + radius * sin(angle), 0), color));
            if (i > 0)
                idx.insert(idx.end(), {
                    (unsigned int)0,
                    (unsigned int)i,
                    (unsigned int)(i + 1)
                    });
        }
        Mesh* eye = new Mesh("eye");
        eye->InitFromData(verts, idx);
        AddMeshToList(eye);
    }

    // picior si laba
    {
        glm::vec3 color = glm::vec3(1.0f, 0.6f, 0.0f); 
        std::vector<VertexFormat> verts_leg = {
            VertexFormat(glm::vec3(20, -5, 0), color),   
            VertexFormat(glm::vec3(30, -5, 0), color),   
            VertexFormat(glm::vec3(20, -20, 0), color), 
            VertexFormat(glm::vec3(30, -20, 0), color)  
        };
        std::vector<unsigned int> idx_leg = { 0, 1, 2, 1, 3, 2 };
        Mesh* leg = new Mesh("leg");
        leg->InitFromData(verts_leg, idx_leg);
        AddMeshToList(leg);

        std::vector<VertexFormat> verts_foot = {
            VertexFormat(glm::vec3(15, -30, 0), color), 
            VertexFormat(glm::vec3(35, -30, 0), color), 
            VertexFormat(glm::vec3(25, -45, 0), color)  
        };
        std::vector<unsigned int> idx_foot = { 0, 1, 2 };

        Mesh* foot = new Mesh("foot");
        foot->InitFromData(verts_foot, idx_foot);
        AddMeshToList(foot);
    }
     
	// cioc
    {
        glm::vec3 color = glm::vec3(1.0f, 0.6f, 0.0f);
        std::vector<VertexFormat> verts = {
            VertexFormat(glm::vec3(60, 30, 0), color),  
            VertexFormat(glm::vec3(50, 33, 0), color),  
            VertexFormat(glm::vec3(50, 27, 0), color)   
        };
        std::vector<unsigned int> idx = { 0, 1, 2 };
        Mesh* beak = new Mesh("beak");
        beak->InitFromData(verts, idx);
        AddMeshToList(beak);
    }

    //creasta mare
    {
        glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f); 
        std::vector<VertexFormat> verts = {
            VertexFormat(glm::vec3(45, 45, 0), color), 
            VertexFormat(glm::vec3(55, 45, 0), color),  
            VertexFormat(glm::vec3(50, 60, 0), color)  
        };
        std::vector<unsigned int> idx = { 0, 1, 2 };
        Mesh* crest_big = new Mesh("crest_big");
        crest_big->InitFromData(verts, idx);
        AddMeshToList(crest_big);
    }

    // creasta mica
    {
        glm::vec3 color = glm::vec3(0.9f, 0.0f, 0.0f);
        std::vector<VertexFormat> verts = {
            VertexFormat(glm::vec3(47, 42, 0), color),  
            VertexFormat(glm::vec3(53, 42, 0), color),  
            VertexFormat(glm::vec3(50, 50, 0), color)  
        };
        std::vector<unsigned int> idx = { 0, 1, 2 };
        Mesh* crest_small = new Mesh("crest_small");
        crest_small->InitFromData(verts, idx);
        AddMeshToList(crest_small);
    }
    // ou (proiectil)
    {
        std::vector<VertexFormat> eggVerts;
        std::vector<unsigned int> eggIdx;
        int segments = 20;
        float r = 10;
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.9f);
        eggVerts.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
        for (int i = 0; i <= segments; i++) {
            float angle = i * 2 * M_PI / segments;
            eggVerts.push_back(VertexFormat(glm::vec3(r * cos(angle), r * sin(angle), 0), color));
            if (i > 0)
                eggIdx.insert(eggIdx.end(), { (unsigned int)0,
                        (unsigned int)i,
                        (unsigned int)(i + 1)
                    });;

        }
        Mesh* egg = new Mesh("egg");
        egg->InitFromData(eggVerts, eggIdx);
        AddMeshToList(egg);
    }

    // inimi viata
    {
        std::vector<VertexFormat> heartVerts;
        std::vector<unsigned int> heartIdx;
        int steps = 50;  
        // jum dreapta
        for (int i = 0; i <= steps; i++) {
            float t = M_PI * i / steps;  // de la 0 la π
            float x = 16 * pow(sin(t), 3);
            float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);
            heartVerts.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(1.0f, 0.05f, 0.1f)));
        }
        // jum stanga
        for (int i = steps; i >= 0; i--) {
            float t = M_PI * i / steps;
            float x = -16 * pow(sin(t), 3); // oglindire pe axa Y
            float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);
            heartVerts.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(1.0f, 0.05f, 0.1f)));
        }
        for (auto& v : heartVerts) {
            v.position.x *= 1.2f;
            v.position.y *= 1.2f;
        }
        //trunchi jos inima
        glm::vec3 center(0, 0, 0);
        int centerIndex = (int)heartVerts.size();
        heartVerts.push_back(VertexFormat(center, glm::vec3(1.0f, 0.05f, 0.1f)));
        for (int i = 0; i < (int)heartVerts.size() - 1; i++) {
            heartIdx.push_back(centerIndex);
            heartIdx.push_back(i);
            heartIdx.push_back((i + 1) % (heartVerts.size() - 1));
        }
        Mesh* heart = new Mesh("heart");
        heart->InitFromData(heartVerts, heartIdx);
        AddMeshToList(heart);
    }

    // panou dupa inimi
    {
        float panelW = 160.0f;
        float panelH = 50.0f;
        glm::vec3 uiGrayA = glm::vec3(0.6f, 0.6f, 0.6f);
        std::vector<VertexFormat> uiVerts = {
            VertexFormat(glm::vec3(0, 0, 0), uiGrayA), 
            VertexFormat(glm::vec3(panelW, 0, 0), uiGrayA),
            VertexFormat(glm::vec3(panelW, panelH, 0), uiGrayA),
            VertexFormat(glm::vec3(0, panelH, 0), uiGrayA)
        };
        std::vector<unsigned int> uiIdx = { 0, 1, 2, 0, 2, 3 };
        Mesh* ui_panel = new Mesh("ui_panel");
        ui_panel->InitFromData(uiVerts, uiIdx);
        AddMeshToList(ui_panel);
    }

    // cadouri(bonusuri)
    {
        std::vector<VertexFormat> giftVerts = {
            // baza patrat
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0.8f, 0)),
            VertexFormat(glm::vec3(20, 0, 0), glm::vec3(1, 0.8f, 0)),
            VertexFormat(glm::vec3(20, 20, 0), glm::vec3(1, 0.8f, 0)),
            VertexFormat(glm::vec3(0, 20, 0), glm::vec3(1, 0.8f, 0)),

            // linie rosie
            VertexFormat(glm::vec3(9, 0, 0), glm::vec3(1, 0, 0)),    
            VertexFormat(glm::vec3(11, 0, 0), glm::vec3(1, 0, 0)),    
            VertexFormat(glm::vec3(11, 20, 0), glm::vec3(1, 0, 0)),  
            VertexFormat(glm::vec3(9, 20, 0), glm::vec3(1, 0, 0)),    

			// ureche stanga cadou
            VertexFormat(glm::vec3(7, 20, 0), glm::vec3(1, 0, 0)),     
            VertexFormat(glm::vec3(10, 20, 0), glm::vec3(1, 0, 0)),    
            VertexFormat(glm::vec3(8.5, 26, 0), glm::vec3(1, 0, 0)),   

            // u. dreapta
            VertexFormat(glm::vec3(10, 20, 0), glm::vec3(1, 0, 0)),    
            VertexFormat(glm::vec3(13, 20, 0), glm::vec3(1, 0, 0)),  
            VertexFormat(glm::vec3(11.5, 26, 0), glm::vec3(1, 0, 0))   
        };
        std::vector<unsigned int> giftIdx = {
            0, 1, 2, 0, 2, 3,       // baza
            4, 5, 6, 4, 6, 7,       // panglica
            8, 9, 10,               // u st
            11, 12, 13              // u dr
        };
        Mesh* gift = new Mesh("gift");
        gift->InitFromData(giftVerts, giftIdx);
        AddMeshToList(gift);
    }

    {
        // pulpa de pui
        std::vector<VertexFormat> verts = {
            // corpul pulpei
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.55f, 0.27f, 0.07f)),  
            VertexFormat(glm::vec3(15, 0, 0), glm::vec3(0.55f, 0.27f, 0.07f)),
            VertexFormat(glm::vec3(15, 15, 0), glm::vec3(0.55f, 0.27f, 0.07f)),
            VertexFormat(glm::vec3(0, 15, 0), glm::vec3(0.55f, 0.27f, 0.07f)),

            // osul 
            VertexFormat(glm::vec3(5, 15, 0), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(10, 15, 0), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(10, 25, 0), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(5, 25, 0), glm::vec3(1, 1, 1))
        };
        std::vector<unsigned int> idx = {
            0, 1, 2, 0, 2, 3, // pulpa
            4, 5, 6, 4, 6, 7  // osu
        };
        Mesh* copanel = new Mesh("copanel");
        copanel->InitFromData(verts, idx);
        AddMeshToList(copanel);
    }

    // particule lovitura 
    {
        std::vector<VertexFormat> verts = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1.0f, 0.6f, 0.1f)), 
            VertexFormat(glm::vec3(4, 0, 0), glm::vec3(1.0f, 0.6f, 0.1f)),
            VertexFormat(glm::vec3(4, 4, 0), glm::vec3(1.0f, 0.6f, 0.1f)),
            VertexFormat(glm::vec3(0, 4, 0), glm::vec3(1.0f, 0.6f, 0.1f))
        };
        std::vector<unsigned int> idx = { 0, 1, 2, 0, 2, 3 };
        Mesh* particle = new Mesh("particle");
        particle->InitFromData(verts, idx);
        AddMeshToList(particle);
    }

    // stele fundal
    {
        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(0,  5, 0), glm::vec3(1, 0.9f, 0.6f)),  
            VertexFormat(glm::vec3(5,  0, 0), glm::vec3(1, 0.9f, 0.6f)),  
            VertexFormat(glm::vec3(0, -5, 0), glm::vec3(1, 0.9f, 0.6f)),  
            VertexFormat(glm::vec3(-5, 0, 0), glm::vec3(1, 0.9f, 0.6f))    
        };
        std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3};
        Mesh* starMesh = new Mesh("star");
        starMesh->InitFromData(vertices, indices);
        AddMeshToList(starMesh);
    }
    // generare stele fundal 
    int numStars = 200;
    for (int i = 0; i < numStars; i++) {
        float x = rand() % (int)resolution.x;
        float y = rand() % (int)resolution.y;
        stars.push_back(glm::vec2(x, y));
    }
}


void Tema1::DrawChicken(glm::vec2 position)
{
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(position.x, position.y);
    modelMatrix *= transform2D::Scale(1.0f, 1.0f);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

    glm::mat3 rightWing = modelMatrix;
    rightWing *= transform2D::Translate(75, 28);  
    rightWing *= transform2D::Rotate(glm::radians(110.0f));
    RenderMesh2D(meshes["wing_left"], shaders["VertexColor"], rightWing);

    glm::mat3 leftWing = modelMatrix;
    leftWing *= transform2D::Translate(-35, 22); 
    leftWing *= transform2D::Rotate(glm::radians(-20.0f)); 
    RenderMesh2D(meshes["wing_left"], shaders["VertexColor"], leftWing);

    glm::mat3 head = modelMatrix;
    head *= transform2D::Translate(-55, 10);
	head *= transform2D::Scale(1.5f, 1.5);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], head);

	glm::mat3 creasta_mica = modelMatrix;
	creasta_mica *= transform2D::Translate(-67, 20);
    creasta_mica *= transform2D::Scale(1.5f, 1.5);
	creasta_mica *= transform2D::Rotate(glm::radians(-10.0f));
    RenderMesh2D(meshes["crest_small"], shaders["VertexColor"], creasta_mica);

    glm::mat3 creasta_mare = modelMatrix;
    creasta_mare *= transform2D::Translate(-32, 32);
    creasta_mare *= transform2D::Rotate(glm::radians(-10.0f));
    RenderMesh2D(meshes["crest_big"], shaders["VertexColor"], creasta_mare);

    glm::mat3 foot_s = modelMatrix;
    foot_s *= transform2D::Translate(-20, 19);
    RenderMesh2D(meshes["foot"], shaders["VertexColor"], foot_s);

    glm::mat3 foot_d = modelMatrix;
    foot_d *= transform2D::Translate(10, 23);
    RenderMesh2D(meshes["foot"], shaders["VertexColor"], foot_d);

    glm::mat3 leg_d = modelMatrix;
    leg_d *= transform2D::Translate(10, 9);
    leg_d *= transform2D::Scale(0.9f, 0.9f);
    RenderMesh2D(meshes["leg"], shaders["VertexColor"], leg_d);

    glm::mat3 leg_s = modelMatrix;
    leg_s *= transform2D::Translate(-14, 6);
    leg_s *= transform2D::Scale(0.9f, 0.9f);
    RenderMesh2D(meshes["leg"], shaders["VertexColor"], leg_s);
    
    glm::mat3 beak = modelMatrix;
    beak *= transform2D::Translate(-45, 28);
    beak *= transform2D::Scale(1.2f, 1.2f);
    beak *= transform2D::Rotate(glm::radians(-10.0f));
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], beak);

    glm::mat3 eye_s = modelMatrix;
    eye_s *= transform2D::Translate(-40, 25);
    RenderMesh2D(meshes["eye"], shaders["VertexColor"], eye_s);

    glm::mat3 eye_d = modelMatrix;
    eye_d *= transform2D::Translate(-28, 27);
    RenderMesh2D(meshes["eye"], shaders["VertexColor"], eye_d);
}


void Tema1::FrameStart()
{
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // desen fundal stele
    for (auto& pos : stars) {
        pos.y -= starSpeed * deltaTimeSeconds;
        if (pos.y < 0)
            pos.y = resolution.y;

        glm::mat3 model = glm::mat3(1);
        model *= transform2D::Translate(pos.x, pos.y);
        model *= transform2D::Scale(0.4f, 0.4f);

        float flicker = 0.3f + 0.7f * fabs(sin(glfwGetTime() * 6.0f + pos.x * 0.15f));
        float tint = 0.85f + 0.15f * sin(glfwGetTime() * 2.0f + pos.y * 0.05f);
        glm::vec3 color = glm::vec3(1.0f * flicker, tint * flicker, 0.7f * flicker);

        RenderMesh2D(meshes["star"], shaders["VertexColor"], model);
    }

    if (gameState == EDITOR) {

        RenderEditorMode(deltaTimeSeconds);

    }else if (gameState == JOC) {

        // camera shake
        if (cameraShakeActive) {
            cameraShakeTime += deltaTimeSeconds;
            float progress = cameraShakeTime / cameraShakeDuration;
            // random vibrare
            float offsetX = ((rand() % 200 - 100) / 100.0f) * cameraShakeIntensity * (1.0f - progress);
            float offsetY = ((rand() % 200 - 100) / 100.0f) * cameraShakeIntensity * (1.0f - progress);
            auto camera = GetSceneCamera();
            camera->SetPosition(glm::vec3(offsetX, offsetY, 50)); 
            camera->Update();

            if (cameraShakeTime >= cameraShakeDuration) {
                cameraShakeActive = false;
                camera->SetPosition(glm::vec3(0, 0, 50)); 
                camera->Update();
            }
        }

        for (auto& p : navaInJoc) {
            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(p.pozitie.x, p.pozitie.y);
            modelMatrix *= transform2D::Scale(shipScale, shipScale);
            for (auto& comp : p.componente) {
                if (comp.first == "linie") { 
                    pozTun = p.pozitie + comp.second;
                }
                std::string meshName = comp.first;
                glm::vec2 offset = comp.second;
                glm::mat3 model = modelMatrix * transform2D::Translate(offset.x, offset.y);
                RenderMesh2D(meshes[meshName], shaders["VertexColor"], model);
            }
        }
        // desen proiectile
        for (auto& p : proiectile) {
            if (!p.active) continue;
            p.position.y += p.speed * deltaTimeSeconds;
			// sterg dace iese din ecran
            float shipHalfHeight = 20.0f;    
            float topBound = resolution.y - shipHalfHeight;
            if (p.position.y > topBound) {
                p.active = false; // dispare cadn ajunge la limita ecran
                continue;
            }

            // desenare pewpew tun
            glm::mat3 model = glm::mat3(1);
            model *= transform2D::Translate(p.position.x, p.position.y);
            model *= transform2D::Scale(0.2f, 0.5f);
            RenderMesh2D(meshes["square"], shaders["VertexColor"], model);
        }

        // desen gaini
        timpTragere += deltaTimeSeconds;
        for (auto& g : inamici) {
            if (!g.active) continue;
            // cobor incet
            g.position.y -= g.speed * deltaTimeSeconds;
            float t = (float)Engine::GetElapsedTime();
            g.position.x = g.startPosition.x + g.amplitude * sin(g.frequency * t + g.phase);
            DrawChicken(g.position);
            // daca aing jos ecran - gata joc
            if (g.position.y <40.0f) {
                gameState = GAME_OVER;
                return; 
            }

            float fundGainii = g.position.y - 30.0f; 
            float inaltimeGainii = 40.0f;
            bool poateTrage = (fundGainii < resolution.y && g.position.y > -100);
            float dist = glm::distance(g.position, navaPosition);
            float shipRadius = 90.0f * shipScale; 
			//daca ma loveste gaina
            if (fabs(g.position.x - navaPosition.x) < shipRadius &&
                fabs(g.position.y - navaPosition.y) < shipRadius) { 
                vieti--;           
                cameraShakeActive = true;
                cameraShakeTime = 0.0f;
                int nrParticule = 20;
				// efect particule explozie
                for (int i = 0; i < nrParticule; i++) {
                    ParticulaExplozie p;
                    p.position = navaPosition;
                    // directie random 
                    float angle = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
                    float speed = 200.0f + ((float)rand() / RAND_MAX) * 200.0f;
                    p.velocity = glm::vec2(cos(angle), sin(angle)) * speed;
                    p.rotation = ((float)rand() / RAND_MAX) * 6.28f;
                    p.lifetime = 0.6f + ((float)rand() / RAND_MAX) * 0.3f; 
                    p.active = true;
                    particuleExplozie.push_back(p);
                }
				// reset pozitia navei
                navaPosition = navaStartPosition;

                if (!navaInJoc.empty()) {
                    navaInJoc[0].pozitie = navaStartPosition;
                }

                if (vieti <= 0) {
                    gameState = GAME_OVER;
                }
            }

            // probabilitate tragere ou
            if (poateTrage) {
                float sansaTragere = ((float)rand() / RAND_MAX); 
                if (sansaTragere < 0.002f) { 
                    ProiectilInamic ou;
                    ou.position = g.position + glm::vec2(20, -inaltimeGainii / 2.0f - 10.0f); 
                    ou.speed = 200.0f;
                    ou.active = true;
                    proiectileInamici.push_back(ou);
                }
            }

			// daca iese din ecran
            if (g.position.y < -50) {
                g.active = false;
            }
        }
        //hit gaina
        for (auto& glont : proiectile) {
            if (!glont.active) continue;
            for (auto& g : inamici) {
                if (!g.active) continue;
                float dist = glm::distance(glont.position, g.position);
                if (dist < 50.0f) {
                    glont.active = false;
                    g.active = false;
                    // efect sare carne la hit gaina
                    ParticulaCopanel p;
                    p.position = g.position;
                    float dir = (rand() % 2 == 0) ? 1.0f : -1.0f;
                    p.velocity = glm::vec2(120.0f * dir, 220.0f);
                    p.rotation = glm::radians(15.0f * dir);
                    p.angularSpeed = glm::radians(250.0f * dir);  
                    p.active = true;
                    copanei.push_back(p);

                    scor += 100;
					// posibil cadou bonus
                    float chanceGift = 0.17f;
                    if ((float)rand() / RAND_MAX < chanceGift) {
                        ProiectilInamic gift;
                        gift.position = g.position + glm::vec2(0.0f, -40.0f);
                        gift.speed = 150.0f;
                        gift.active = true;
                        gift.isGift = true;
                        gift.points = 1000;
                        proiectileInamici.push_back(gift);
                    }
                    break;
                }
            }
        }

        // reste timer dupa tragere ou 
        if (timpTragere >= intervalTragere) {
            timpTragere = 0.0f;
        }

        // desenare ou
        for (auto& o : proiectileInamici) {
            if (!o.active) continue;
			// bounce ou pe bumper
            if (o.bouncing) {
                o.position += o.velocity * deltaTimeSeconds;
                o.velocity.y -= 900.0f * deltaTimeSeconds; 
                o.rotation += 6.0f * deltaTimeSeconds;   
                if (o.position.y < -50)
                    o.active = false;
            }
            else {
                o.position.y -= o.speed * deltaTimeSeconds;
                if (o.position.y < 0) {
                    o.active = false;
                    continue;
                }
            }

			// detectie coliziune ou cu bumper
            if (!o.isGift) {
                for (auto& p : navaInJoc) {
                    for (auto& comp : p.componente) {
                        if (comp.first == "patrat_ciuperca") { 
                            glm::vec2 bumperPos = p.pozitie + comp.second;

                            float bumperW = 3 * cellSize;
                            float bumperH = 2 * cellSize;
                            float bumperLeft = bumperPos.x - cellSize;
                            float bumperRight = bumperLeft + bumperW;
                            float bumperBottom = bumperPos.y;
                            float bumperTop = bumperBottom + bumperH - 40.0f;

                            if (o.position.x >= bumperLeft && o.position.x <= bumperRight &&
                                o.position.y >= bumperBottom && o.position.y <= bumperTop) {
                                // trimis la stanga dreapta random
                                float dirX = (rand() % 2 == 0) ? 1.0f : -1.0f;  
                                float angle = glm::radians(60.0f + (rand() % 20 - 10)); 
                                float speed = 250.0f + (rand() % 100); 
                                glm::vec2 v;
                                v.x = cos(angle) * speed * dirX;
                                v.y = sin(angle) * speed; 

                                // mem comp de bounce
                                o.bouncing = true;
                                o.velocity = v; 
                                o.rotation = glm::radians((float)(rand() % 360));
                                o.position.y = bumperTop;

                                // efect particula la atingere
                                ParticulaExplozie spark;
                                spark.position = o.position;
                                spark.velocity = glm::vec2(0, 250);
                                spark.lifetime = 0.2f;
                                spark.active = true;
                                particuleExplozie.push_back(spark);
                            }
                        }
                    }
                }
            }

            // verif element din contact ou sua cadou
            float dist = glm::distance(o.position, navaPosition);
            if (dist < 50.0f) {
                o.active = false;
                if (o.isGift) {
                    scor += o.points;
                }
                else {
                    vieti--;
                    // explozie coliz cu ou
                    int nrParticule = 20;
                    for (int i = 0; i < nrParticule; i++) {
                        ParticulaExplozie p;
                        p.position = navaPosition;

                        float angle = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
                        float speed = 200.0f + ((float)rand() / RAND_MAX) * 200.0f;
                        p.velocity = glm::vec2(cos(angle), sin(angle)) * speed;

                        p.rotation = ((float)rand() / RAND_MAX) * 6.28f;
                        p.lifetime = 0.6f + ((float)rand() / RAND_MAX) * 0.3f;
                        p.active = true;
                        particuleExplozie.push_back(p);
                    }
                    cameraShakeActive = true;
                    cameraShakeTime = 0.0f;
                    // respawn nava 
                    navaPosition = navaStartPosition;
                    if (!navaInJoc.empty()) {
                        navaInJoc[0].pozitie = navaStartPosition;
                    }
                    if (vieti <= 0) {
                        gameState = GAME_OVER; 
                    }
                }
                continue;  
            }

            // desenare ou si cadou
            glm::mat3 model = glm::mat3(1);
            model *= transform2D::Translate(o.position.x, o.position.y);
            if (o.isGift) {
                model *= transform2D::Scale(1.6f, 1.6f);
                RenderMesh2D(meshes["gift"], shaders["VertexColor"], model);
            }
            else {
                model *= transform2D::Scale(0.9f, 1.2f);
                RenderMesh2D(meshes["egg"], shaders["VertexColor"], model);
            }
        }

        // copanel
        for (auto& p : copanei) {
            if (!p.active) continue;
            p.position += p.velocity * deltaTimeSeconds;
            p.velocity.y -= 900.0f * deltaTimeSeconds; 
            p.rotation += p.angularSpeed * deltaTimeSeconds;

            // desen
            glm::mat3 model = glm::mat3(1);
            model *= transform2D::Translate(p.position.x, p.position.y);
            model *= transform2D::Rotate(p.rotation);
            RenderMesh2D(meshes["copanel"], shaders["VertexColor"], model);
            if (p.position.y < -50)
                p.active = false;
        }

        // particule explozie
        for (auto& p : particuleExplozie) {
            if (!p.active) continue;
            p.position += p.velocity * deltaTimeSeconds;
            p.velocity *= 0.93f;                    
            p.velocity.y -= 600.0f * deltaTimeSeconds; 
            p.rotation += 8.0f * deltaTimeSeconds;
            p.lifetime -= deltaTimeSeconds;
            // desenare
            glm::mat3 model = glm::mat3(1);
            model *= transform2D::Translate(p.position.x, p.position.y);
            model *= transform2D::Rotate(p.rotation);
            RenderMesh2D(meshes["particle"], shaders["VertexColor"], model);
            if (p.lifetime <= 0)
                p.active = false;
        }
        // desen panou vieti dupa
        float pad = 0.0f;
        float panelX = pad;
        float panelY = resolution.y - (40.0f + pad);

        glm::mat3 model = glm::mat3(1);
        model *= transform2D::Translate(panelX, panelY);

        RenderMesh2D(meshes["ui_panel"], shaders["VertexColor"], model);
        
        float startX_heart = 20.0f;
        float startY_heart = resolution.y - 20.0f;
        float distBetween = 40.0f;

        for (int i = 0; i < vieti; i++) {
            glm::mat3 modelHeart = glm::mat3(1);
            modelHeart *= transform2D::Translate(startX_heart + i * distBetween, startY_heart);
            modelHeart *= transform2D::Scale(0.7f, 0.7f);
            RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelHeart);
        }

        // verif cati inamici mai sunt
        int activi = 0;
        for (auto& g : inamici)
            if (g.active)
                activi++;

        // daca nu gata, urm val
        if (activi == 0 && !asteaptaUrmatorulVal) {
            asteaptaUrmatorulVal = true;
            waveTimer = 0.0f; 
        }

        // sistem valuri
        if (asteaptaUrmatorulVal) {
            waveTimer += deltaTimeSeconds;
            std::string mesajWave;
            if (waveNumber < 3) {
                mesajWave = "WAVE " + std::to_string(waveNumber + 1);
            }
            else {
                scorFinal = scor;

            }
            float puls = 0.9f + 0.1f * sin(4.0f * Engine::GetElapsedTime());
            textRenderer->RenderText(
                mesajWave,
                resolution.x / 2.0f - 130.0f,
                resolution.y / 2.0f,
                1.6f*puls,
                glm::vec3(1, 1, 0)
            );
            if (waveTimer >= 5.0f) {
                asteaptaUrmatorulVal = false;
                waveNumber++;
                if (waveNumber <= 3) {
                    GenereazaInamici();  //urmat val
                } else {
                    gameState = GAME_WIN;
                }
            }
        }

        textRenderer->RenderText("SCORE: " + std::to_string(scor),
            10.0f,                   
            resolution.y - 670.0f,      
            0.3f,                     
            glm::vec3(1.0f, 0.9f, 0.2f)); 

    } else if (gameState == GAME_OVER) {
         glClearColor(0, 0, 0, 1);
         glClear(GL_COLOR_BUFFER_BIT);
         std::string msg = "GAME OVER";
         std::string scoreMsg = "SCORE: " + std::to_string(scor);
         textRenderer->RenderText(msg, resolution.x / 2 - 320, resolution.y / 2, 2.0f, glm::vec3(1, 0, 0));
         textRenderer->RenderText(scoreMsg, resolution.x / 2 - 250, resolution.y / 2 - 100, 1.5f, glm::vec3(1, 1, 1));

         float centerX = resolution.x / 2.0f;
         float centerY = resolution.y / 2.0f;
         glm::mat3 modelBox = glm::mat3(1);
         modelBox *= transform2D::Translate(centerX - 250, centerY - 80);
         modelBox *= transform2D::Scale(500, 160);
         RenderMesh2D(meshes["ui_bg"], shaders["VertexColor"], modelBox);

    } else if (gameState == GAME_WIN) {
        glClearColor(0.1f, 0.0f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        std::string mesaj1 = "YOU WIN!";
        std::string mesaj2 = "FINAL SCORE: " + std::to_string(scorFinal);
        textRenderer->RenderText(
            mesaj1,
            resolution.x / 2.0f - 200.0f,
            resolution.y / 2.0f + 30.0f,
            1.5f,
            glm::vec3(1, 1, 0)
        );
        textRenderer->RenderText(
            mesaj2,
            resolution.x / 2.0f - 350.0f,
            resolution.y / 2.0f - 50.0f,
            1.2f,
            glm::vec3(1, 1, 1)
        );

    } else if (gameState == PAUZA) {
        glClearColor(0.05f, 0.05f, 0.05f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        std::string msg = "PAUSED";
        std::string hint = "Press P to resume";

        textRenderer->RenderText(
            msg,
            resolution.x / 2.0f - 180.0f,
            resolution.y / 2.0f + 30.0f,
            1.4f,
            glm::vec3(1, 1, 0)
        );
        textRenderer->RenderText(
            hint,
            resolution.x / 2.0f - 190.0f,
            resolution.y / 2.0f - 60.0f,
            0.8f,
            glm::vec3(1, 1, 1)
        );
    }
}


void Tema1::RenderEditorMode(float deltaTimeSeconds)
{
    // grid
    float startX = (resolution.x - coloane * cellSize) / 2 + 155;
    float startY = (resolution.y - randuri * cellSize) / 2 - 75;

    float gridWidth = coloane * cellSize;
    float gridHeight = randuri * cellSize;

    float innerLeft = startX;
    float innerRight = startX + gridWidth;
    float innerBottom = startY;
    float innerTop = startY + gridHeight;

    float offsetX = 26;
    float offsetY = 20;

    float borderLeft = innerLeft - offsetX;
    float borderRight = innerRight + offsetX;
    float borderBottom = innerBottom - offsetY;
    float borderTop = innerTop + offsetY;

    float gap = 14;
    float innerSize = cellSize - gap;

    for (int i = 0; i < randuri; i++) {
        for (int j = 0; j < coloane; j++) {
            float x = startX + j * cellSize + gap / 2;
            float y = startY + i * cellSize + gap / 2;

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y);
            modelMatrix *= transform2D::Scale(innerSize / 50, innerSize / 50);

            RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
        }
    }

    // chenar albastru
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(borderLeft, borderBottom);
    modelMatrix *= transform2D::Scale(borderRight - borderLeft, 1);
    RenderMesh2D(meshes["line"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(borderLeft, borderTop);
    modelMatrix *= transform2D::Scale(borderRight - borderLeft, 1);
    RenderMesh2D(meshes["line"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(borderLeft, borderBottom);
    modelMatrix *= transform2D::Rotate(M_PI_2);
    modelMatrix *= transform2D::Scale(borderTop - borderBottom, 1);
    RenderMesh2D(meshes["line"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(borderRight, borderBottom);
    modelMatrix *= transform2D::Rotate(M_PI_2);
    modelMatrix *= transform2D::Scale(borderTop - borderBottom, 1);
    RenderMesh2D(meshes["line"], shaders["VertexColor"], modelMatrix);

    // liniile rosii
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(300, 0);
    modelMatrix *= transform2D::Rotate(M_PI_2);
    modelMatrix *= transform2D::Scale(resolution.y, 1);
    RenderMesh2D(meshes["red_line"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(0, 170);
    modelMatrix *= transform2D::Scale(300, 1);
    RenderMesh2D(meshes["red_line"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(0, 360);
    modelMatrix *= transform2D::Scale(300, 1);
    RenderMesh2D(meshes["red_line"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(0, 550);
    modelMatrix *= transform2D::Scale(300, 1);
    RenderMesh2D(meshes["red_line"], shaders["VertexColor"], modelMatrix);

    // patrate verzi sus
    float startX_verde = (resolution.x - col_verzi * cellSize_verde) / 2.f + 100;
    float startY_verde = (resolution.y - rand_verzi * cellSize_verde) / 2.f + 280;
    float gap_v = 60;
    float innerSize_v = cellSize_verde - gap_v;

    for (int i = 0; i < availablePieces && i < col_verzi; i++) {
        float x_v = startX_verde + i * cellSize_verde + gap_v / 2;
        float y_v = startY_verde + gap_v / 2;

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x_v, y_v);
        modelMatrix *= transform2D::Scale(innerSize_v / 50, innerSize_v / 50);
        RenderMesh2D(meshes["green_square"], shaders["VertexColor"], modelMatrix);
    }

    // buton start
    float gap_button = 30;
    float x_arrow = resolution.x - cellSize_verde + gap_button;
    float y_arrow = startY_verde + gap_v / 2.f;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x_arrow, y_arrow);
    modelMatrix *= transform2D::Scale(innerSize_v / 50, innerSize_v / 50);
    RenderMesh2D(meshes["arrow_square"], shaders["VertexColor"], modelMatrix);

    // desenare piese pe grid
    float scaleOnGrid = 1.0f + (gap / cellSize);
    float offsetYFix = (cellSize * (scaleOnGrid - 1)) * 0.5f;

    auto RenderPiesa = [&](const Piesa& piesa) {
        for (auto& comp : piesa.componente) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(
                piesa.pozitie.x + comp.second.x,
                piesa.pozitie.y + comp.second.y
            );
            RenderMesh2D(meshes[comp.first], shaders["VertexColor"], modelMatrix);
        }
        };

    for (auto& p : piese) {
        RenderPiesa(p);
    }

    if (hasDragPiece) {
        RenderPiesa(piesaDrag);
    }
}

void Tema1::GenereazaInamici() {
    // conf rand col inamici
    int numarRanduri = 3;
    int numarColoane = 8;
    float distX = 140.0f;
    float distY = 120.0f;
    float startY_gaini = 850.0f;
    float totalWidth = (numarColoane - 1) * distX;
    float startX_gaini = (resolution.x - totalWidth) / 2.0f;
    inamici.clear();

    // increase dificult in fct de wave
    float vitezaCoborare = 25.0f + 10.0f * (waveNumber - 1);
    float vitezaProiectil = 200.0f + 50.0f * (waveNumber - 1);
    float sansaTragere = 0.002f + 0.001f * (waveNumber - 1);

    for (int i = 0; i < numarRanduri; i++) {
        float fazaRand = i * 1.2f;
        float frecventaRand = 2.0f;
        float amplitudineRand = 50.0f;
        for (int j = 0; j < numarColoane; j++) {
            Inamic g;
            g.position = glm::vec2(startX_gaini + j * distX, startY_gaini - i * distY);
            g.startPosition = g.position;
            g.active = true;
            g.phase = fazaRand;
            g.amplitude = amplitudineRand;
            g.frequency = frecventaRand;
            g.speed = vitezaCoborare;
            inamici.push_back(g);
        }
    }
}

void Tema1::FrameEnd()
{
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (gameState != JOC)
        return;

    if (gameState == JOC) {
        // numar cate motoare
        int nrMotoare = 0;
        if (!navaInJoc.empty()) {
            for (auto& comp : navaInJoc[0].componente) {
                if (comp.first == "patrat_oranj") {
                    nrMotoare++;
                }
            }
        }

        // set viteza
        float vitezaBaza = 120.0f;
        float vitezaFinala = vitezaBaza * (1.0f + 0.5f * nrMotoare);
        float speed = vitezaFinala * deltaTime;

        if (window->KeyHold(GLFW_KEY_LEFT)) {
            navaPosition.x -= speed;
        }
        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            navaPosition.x += speed;
        }
        if (window->KeyHold(GLFW_KEY_UP)) {
            navaPosition.y += speed;
        }
        if (window->KeyHold(GLFW_KEY_DOWN)) {
            navaPosition.y -= speed;
        }

        float shipHalfWidth = 90.0f;   
        float shipHalfHeight = 60.0f;   
        float leftBound = 0.0f + shipHalfWidth;
        float rightBound = resolution.x - shipHalfWidth;   
        float bottomBound = 0.0f + shipHalfHeight;
        float topBound = resolution.y - shipHalfHeight;

        if (navaPosition.x < leftBound)
            navaPosition.x = leftBound;
        if (navaPosition.x > rightBound)
            navaPosition.x = rightBound;
        if (navaPosition.y < bottomBound)
            navaPosition.y = bottomBound;
        if (navaPosition.y > topBound)
            navaPosition.y = topBound;
        if (!navaInJoc.empty()) {
            navaInJoc[0].pozitie = navaPosition;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // impusc din space
    if (key == GLFW_KEY_SPACE) {
        for (auto& p : navaInJoc) {
            for (auto& comp : p.componente) {
                if (comp.first == "linie") { 
                    // construiesc EXACT aceeasi matrice ca la desen 
                    glm::mat3 M = glm::mat3(1);
                    M *= transform2D::Translate(p.pozitie.x, p.pozitie.y);
                    M *= transform2D::Scale(shipScale, shipScale);
                    M *= transform2D::Translate(comp.second.x, comp.second.y);
                    glm::vec3 local(0, 0, 1);
                    // transf in coord de ecr
                    glm::vec3 world3 = M * local;
                    glm::vec2 spawn(world3.x, world3.y);
                    proiectile.push_back({ spawn, 600.0f, true });
                }
            }
        }
    }
    // pauza joc
    if (key == GLFW_KEY_P) {
        if (gameState == JOC) {
            gameState = PAUZA;
        }
        else if (gameState == PAUZA) {
            gameState = JOC;
        }
    }
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    mouseY = resolution.y - mouseY;
    glm::vec2 mousePos(mouseX, mouseY);

    if (dragging && hasDragPiece) {
        piesaDrag.pozitie = mousePos - offsetDrag;
    }
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    mouseY = resolution.y - mouseY; 
    glm::vec2 mousePos(mouseX, mouseY);

    float width = 120 * (resolution.x / 1280.0f);
    float height = 120 * (resolution.y / 720.0f);
    int scaleX = resolution.x / 1280;
    int scaleY = resolution.y / 720;
    float startX = (resolution.x - coloane * cellSize) / 2 + 150;
    float startY = (resolution.y - randuri * cellSize) / 2 - 80;
    float gridWidth = coloane * cellSize;
    float gridHeight = randuri * cellSize;

    // butonu start
    float startX_verde = (resolution.x - col_verzi * cellSize_verde) / 2.f + 100;
    float startY_verde = (resolution.y - rand_verzi * cellSize_verde) / 2.f + 280;
    float gap_v = 60;
    float innerSize_v = cellSize_verde - gap_v;
    float gap_button = 30;
    float x_arrow = resolution.x - cellSize_verde + gap_button;
    float y_arrow = startY_verde + gap_v / 2.f;

    if (mousePos.x >= x_arrow && mousePos.x <= x_arrow + cellSize_verde &&
        mousePos.y >= y_arrow && mousePos.y <= y_arrow + cellSize_verde)
    {
        navaBuna = VerificaNava();
        if (navaBuna) {
            startButtonColor = glm::vec3(0, 1, 0);   
            gameState = JOC;
            inamici.clear();

			GenereazaInamici();

            float startX = (resolution.x - coloane * cellSize) / 2 + 150;
            float startY = (resolution.y - randuri * cellSize) / 2 - 80;
            float gridWidth = coloane * cellSize;
            float gridHeight = randuri * cellSize;

            float sumX = 0, sumY = 0;
            int count = 0;
            for (auto& p : piese) {
                bool inGrid = p.pozitie.x >= startX && p.pozitie.x <= startX + gridWidth &&
                    p.pozitie.y >= startY && p.pozitie.y <= startY + gridHeight;
                if (inGrid) {
                    sumX += p.pozitie.x;
                    sumY += p.pozitie.y;
                    count++;
                }
            }
            if (count == 0) return; 
            glm::vec2 center(sumX / count, sumY / count);

            // creez piesa comnbinata
            Piesa navaFinala;
            navaFinala.pozitie = glm::vec2(resolution.x / 2.0f, resolution.y / 4.0f);
            navaFinala.scaraX = 1.0f;
            navaFinala.scaraY = 1.0f;

            for (auto& p : piese) {
                bool inGrid = p.pozitie.x >= startX && p.pozitie.x <= startX + gridWidth &&
                    p.pozitie.y >= startY && p.pozitie.y <= startY + gridHeight;
                if (!inGrid)
                    continue;
                for (auto& comp : p.componente) {
                    glm::vec2 pozGlobala = p.pozitie + comp.second * glm::vec2(p.scaraX, p.scaraY);
                    glm::vec2 offset = pozGlobala - center;
                    navaFinala.componente.push_back({ comp.first, offset });
                }
            }
            navaInJoc.clear();
            navaInJoc.push_back(navaFinala);
            piese.clear();
            // set pos de start
            navaPosition = glm::vec2(resolution.x / 2.0f, resolution.y / 4.0f);
            navaInJoc[0].pozitie = navaPosition;
            piese.clear();
            navaPosition = glm::vec2(resolution.x / 2.0f, resolution.y / 4.0f);
            
            float centerX = resolution.x / 2.0f;
            float centerY = resolution.y / 4.0f;
            for (auto& p : navaInJoc) {
                float dx = centerX - p.pozitie.x;
                float dy = centerY - p.pozitie.y;
                p.pozitie += glm::vec2(dx, dy);
            }
            navaPosition = glm::vec2(centerX, centerY);

        } else {
            startButtonColor = glm::vec3(1, 0, 0);  
        }

        float c = cellSize_verde;
        std::vector<VertexFormat> verts = {
                VertexFormat(glm::vec3(c, c, 0), startButtonColor),
                VertexFormat(glm::vec3(0, c, 0), startButtonColor),
                VertexFormat(glm::vec3(0, 0, 0), startButtonColor),
                VertexFormat(glm::vec3(c, 0, 0), startButtonColor),
                VertexFormat(glm::vec3(c / 2, c / 2, 0), startButtonColor)
        };
        std::vector<unsigned int> idx_fig = { 0, 1, 4, 1, 2, 4, 2, 3, 4 };
        Mesh* arrow_square = new Mesh("arrow_square");
        arrow_square->InitFromData(verts, idx_fig);
        AddMeshToList(arrow_square);
        return;
    }
        // sterge de pe grid
        if ( button == 2) {
            for (int i = (int)piese.size() - 1; i >= 0; i--) {
                auto& p = piese[i];
                auto box = verifPiese(p);
                float x = box.x, y = box.y, w = box.z, h = box.w;
                bool insidePiece = mousePos.x >= x && mousePos.x <= x + w &&
                    mousePos.y >= y && mousePos.y <= y + h;
                bool insideGrid = mousePos.x >= startX && mousePos.x <= startX + gridWidth &&
                    mousePos.y >= startY && mousePos.y <= startY + gridHeight;

                if (insidePiece && insideGrid) {
                    piese.erase(piese.begin() + i);
                    if (availablePieces < 10) {
                        availablePieces++;
                        UpdateStartButton();
                    }
                    return;
                }
            }
            return; 
        }
        // add piesa pe grid
        if (button == 1) {
            if (availablePieces <= 0) {
                return;
            }
            for (auto& p : piese) {
                bool insidePiece = mousePos.x >= p.pozitie.x && mousePos.x <= p.pozitie.x + width &&
                    mousePos.y >= p.pozitie.y && mousePos.y <= p.pozitie.y + height;
                if (insidePiece && p.pozitie.x < 300) {
                    piesaDrag = p;
                    piesaDrag.pozitie = mousePos - glm::vec2(20 * scaleX, 20 * scaleY);
                    dragging = true;
                    hasDragPiece = true;

                    float halfW = 20 * scaleX;
                    float halfH = 20 * scaleY;
                    offsetDrag = mousePos - (p.pozitie + glm::vec2(halfW, halfH));
                    return;
                }
            }
        }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (button == 1) {
        mouseY = resolution.y - mouseY;
        dragging = false;

        if (hasDragPiece) {
            hasDragPiece = false;
            int scaleX = resolution.x / 1280;
            int scaleY = resolution.y / 720;

            // calc cel din grid
            float startX = (resolution.x - coloane * cellSize) / 2 + 150;
            float startY = (resolution.y - randuri * cellSize) / 2 - 80;
            float gridWidth = coloane * cellSize;
            float gridHeight = randuri * cellSize;

            // daca mouse e in grid
            bool insideGrid = mouseX >= startX && mouseX <= startX + gridWidth && mouseY >= startY && mouseY <= startY + gridHeight;
            if (insideGrid) {
                if (availablePieces <= 0) {
                    return;
                }
                // pun piesa pe grid
                int col = (piesaDrag.pozitie.x - startX) / cellSize;
                int row = (piesaDrag.pozitie.y - startY) / cellSize;
                
                col = std::max(0, std::min(col, coloane - 1));
                row = std::max(0, std::min(row, randuri - 1));
                for (auto& c : piesaDrag.componente) {
                    if (c.first == "patrat_oranj") {
                        row += 1;
                    }
                }
                piesaDrag.pozitie.x = startX + col * cellSize + cellSize / 2 - 20 * scaleX;
                piesaDrag.pozitie.y = startY + row * cellSize + cellSize / 2 - 20 * scaleY;

                int inaltime = 1; 
                for (auto& c : piesaDrag.componente) {
                    if (c.first == "tija" || c.first == "semicerc_tun") {
                        inaltime = 3;
                    }
                    else if (c.first == "patrat_oranj") {
                        inaltime = 2; 
                    }
                    else if (c.first == "patrat_ciuperca") {
                        inaltime = 2; 
                    }
                }
                // retin randuri ocupate
                piesaDrag.randuriOcupate.clear();
                for (int h = 0; h < inaltime; h++) {
                    piesaDrag.randuriOcupate.push_back(row + h);
                }

                // =prima pieha ianltimea cel putin 1
                bool regulaIncalcata = false;
                if (availablePieces == 10 && inaltime > 1) {
                    regulaIncalcata = true;
                    navaBuna = false;
                    startButtonColor = glm::vec3(1, 0, 0); 
                }

                bool schimbare = false;
                bool regulaIncalcata_motor = false;
                bool esteMotor = false;

                if (!piesaDrag.componente.empty() && piesaDrag.componente[0].first == "patrat_oranj")
                    esteMotor = true;
                    piese.push_back(piesaDrag);
                    if (availablePieces > 0) {
                        availablePieces--;
                    }
                    UpdateStartButton(); 
            }

        }
    }

}

void Tema1::UpdateStartButton()
{
    navaBuna = VerificaNava();
    if (navaBuna) {
        startButtonColor = glm::vec3(0, 1, 0);  
        navaSalvata.clear();
        for (auto& p : piese) {
            PiesaSalvata ps;
            ps.pozitie = p.pozitie;
            ps.tip = p.componente[0].first;
            navaSalvata.push_back(ps);
        }
    }
    else {
        startButtonColor = glm::vec3(1, 0, 0);  
    }
    float c = cellSize_verde;
    std::vector<VertexFormat> verts = {
        VertexFormat(glm::vec3(c, c, 0), startButtonColor),
        VertexFormat(glm::vec3(0, c, 0), startButtonColor),
        VertexFormat(glm::vec3(0, 0, 0), startButtonColor),
        VertexFormat(glm::vec3(c, 0, 0), startButtonColor),
        VertexFormat(glm::vec3(c / 2, c / 2, 0), startButtonColor)
    };
    std::vector<unsigned int> idx_fig = { 0, 1, 4, 1, 2, 4, 2, 3, 4 };
    Mesh* arrow_square = new Mesh("arrow_square");
    arrow_square->InitFromData(verts, idx_fig);
    AddMeshToList(arrow_square);
}



bool Tema1::overlapFunc(float x1, float y1, float w1, float h1,  float x2, float y2, float w2, float h2) {
    return !(x1 + w1 <= x2 || x2 + w2 <= x1 ||
        y1 + h1 <= y2 || y2 + h2 <= y1);
}


int Tema1::PieceType(const Piesa& p) {
    bool eBloc = false, eMotor = false, eBumper = false, eTun = false;
    for (auto& c : p.componente) {
        if (c.first == "patrat_gri") eBloc = true;
        if (c.first == "patrat_oranj") eMotor = true;
        if (c.first == "patrat_ciuperca") eBumper = true;
        if (c.first == "tija" || c.first == "semicerc_tun") eTun = true;
    }
    if (eTun)   return 4;
    if (eMotor) return 2;
    if (eBumper)return 3;
    if (eBloc)  return 1;
    return 0;
}

glm::vec4 Tema1::verifPiese(const Piesa& p) {
    float cell = cellSize;
    int t = PieceType(p);
    float x = p.pozitie.x;
    float y = p.pozitie.y;
    float w = cell, h = cell;

    switch (t) {
    case 1: // bloc
        w = cell; h = cell;
        break;
    case 4: // tun
        w = cell; h = 3 * cell;
        break;
    case 2: // motor
        w = cell; h = 2 * cell;
        break;
    case 3: // bumper
        w = 3 * cell;
        h = 2 * cell;
        x -= cell;
        y += cell;
        break;
    default:
        w = cell; h = cell;
        break;
    }
    return { x, y, w, h };
}


bool Tema1::VerificaNava()
{
    startButtonColor = glm::vec3(0, 1, 0); 

    // coliziune nava 
    auto pieceType = [&](const Piesa& p)->int {
        // 1=bloc, 2=motor, 3=bumper, 4=tun
        bool eBloc = false, eMotor = false, eBumper = false, eTun = false;
        for (auto& c : p.componente) {
            if (c.first == "patrat_gri") eBloc = true;
            if (c.first == "patrat_oranj") eMotor = true;
            if (c.first == "patrat_ciuperca") eBumper = true;
            if (c.first == "tija" || c.first == "semicerc_tun") eTun = true;
        }
        if (eTun)   return 4;
        if (eMotor) return 2;
        if (eBumper)return 3;
        if (eBloc)  return 1;

        return 0;
        };

    // verif fiecare pereche de piese
    for (size_t i = 0; i < piese.size(); ++i) {
        for (size_t j = i + 1; j < piese.size(); ++j) {
            glm::vec4 A = verifPiese(piese[i]);
            glm::vec4 B = verifPiese(piese[j]);

            if (overlapFunc(A.x, A.y, A.z, A.w, B.x, B.y, B.z, B.w)) {
                return false;
            }
        }
    }

    // coordonate grid
    float startX = (resolution.x - coloane * cellSize) / 2 + 150;
    float startY = (resolution.y - randuri * cellSize) / 2 - 80;

    std::vector<std::vector<int>> grid(randuri, std::vector<int>(coloane, 0));
    std::vector<std::vector<bool>> forbidAbove(randuri, std::vector<bool>(coloane, false));
    std::vector<std::vector<bool>> forbidAboveTun(randuri, std::vector<bool>(coloane, false));
    std::vector<std::vector<bool>> forbidRow(randuri, std::vector<bool>(coloane, false));
    std::vector<int> randuriFlacari;
    // marchez piese si verif suprapunerea
    bool eFlacara = false;

    for (auto& p : piese) {
        int col = (int)floor((p.pozitie.x - startX) / cellSize + 0.5f);
        int row = (int)floor((p.pozitie.y - startY) / cellSize + 0.5f);
        if (row < 0 || row >= randuri || col < 0 || col >= coloane)
            continue;
        int type = 0;
        if (p.componente[0].first == "patrat_gri") type = 1;
        else if (p.componente[0].first == "patrat_oranj") {
            type = 2;
        }
        for (auto& c : p.componente) {
            if (c.first == "patrat_gri") type = 1;
            if (c.first == "patrat_oranj") type = 2;
            if (c.first == "patrat_ciuperca") type = 3;
            if (c.first == "tija" || c.first == "semicerc_tun") type = 4;
        }
		// marcheaza piesa in grid
        grid[row][col] = type;

        if (type == 2) {
            int flameRow = row;
            if (flameRow < randuri)
                grid[flameRow][col] = 2; // flacara e conectat
            // col cu motor e blocata
            if (col > 0) {
                forbidAbove[flameRow][col - 1] = true;
            }
            if (col < coloane - 1) {
                forbidAbove[flameRow][col + 1] = true;
            }
            // rand sub flacara blocat
            int rowBlocked = row -1 ;
            if (rowBlocked < randuri)
                for (int j = 0; j < coloane; j++) {
                    if (j == col) {
                        continue;
                    }
                    forbidRow[rowBlocked][j] = true;
                }
        }

        // ciuperca bloc col laterale
        if (type == 3) {
            for (int d = -1; d <= 1; d++) {
                int c = col + d;
                if (c < 0 || c >= coloane) continue;
                for (int r = row + 1; r < randuri; r++)
                    forbidAbove[r][c] = true;
            }
        }

        // tun - blocheaza toata coloana deasupra bazei
        if (type == 4) {
            for (int r = row + 1; r < randuri; r++) {
                forbidAboveTun[r][col] = true;
            }
        }

}

    // prima piesa - >patrat 
    bool existaPatratGri = false;
    for (auto& p : piese) {
        //piese inside grid
        int col = (int)floor((p.pozitie.x - startX) / cellSize + 0.5f);
        int row = (int)floor((p.pozitie.y - startY) / cellSize + 0.5f);
        if (row < 0 || row >= randuri || col < 0 || col >= coloane)
            continue; 
        for (auto& comp : p.componente) {
            if (comp.first == "patrat_gri") {
                existaPatratGri = true;
                break;
            }
        }
        if (existaPatratGri) break;
    }
    // daca nu e piesa gri patrat
    if (!existaPatratGri) {
        startButtonColor = glm::vec3(1, 0, 0);  
        navaBuna = false;
        float c = cellSize_verde;
        std::vector<VertexFormat> verts = {
            VertexFormat(glm::vec3(c, c, 0), startButtonColor),
            VertexFormat(glm::vec3(0, c, 0), startButtonColor),
            VertexFormat(glm::vec3(0, 0, 0), startButtonColor),
            VertexFormat(glm::vec3(c, 0, 0), startButtonColor),
            VertexFormat(glm::vec3(c / 2, c / 2, 0), startButtonColor)
        };
        std::vector<unsigned int> idx_fig = { 0, 1, 4, 1, 2, 4, 2, 3, 4 };
        Mesh* arrow_square = new Mesh("arrow_square");
        arrow_square->InitFromData(verts, idx_fig);
        AddMeshToList(arrow_square);
        return false;
    }

    for (int r = 0; r < randuri; r++) {
        for (int c = 0; c < coloane; c++) {
            if (grid[r][c] == 0) continue;
            if (forbidRow[r][c]) {
                startButtonColor = glm::vec3(1, 0, 0);
                return false;
            }
            if (forbidAboveTun[r][c]) {  
                startButtonColor = glm::vec3(1, 0, 0);
                return false;
            }
        }
    }

	// ciuperca fara vecini laterali
    bool toateCiupercileOK = true;
    for (int r = 0; r < randuri; r++) {
        for (int c = 1; c + 1 < coloane; c++) {
            if (grid[r][c] == 3 && grid[r][c - 1] == 3 && grid[r][c + 1] == 3) {
                bool stanga = (c - 2 >= 0 && grid[r][c - 2] != 0);
                bool dreapta = (c + 2 < coloane && grid[r][c + 2] != 0);
                if (!(stanga || dreapta)) {
                    toateCiupercileOK = false;
                }
            }
        }
    }
    if (!toateCiupercileOK) {
        startButtonColor = glm::vec3(1, 0, 0);
        return false;
    }

    if (!EsteConexa(grid)) {
        startButtonColor = glm::vec3(1, 0, 0);
        return false;
    }

    startButtonColor = glm::vec3(0, 1, 0);
    return true;
}


bool Tema1::EsteConexa(const std::vector<std::vector<int>>& grid)
{
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    int sr = -1, sc = -1;
    for (int i = 0; i < rows && sr == -1; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] != 0) {
                sr = i;
                sc = j;
                break;
            }
        }
    }

    if (sr == -1) return false; // fara blocuri

    std::vector<std::pair<int, int>> stiva;
    stiva.push_back(std::make_pair(sr, sc));
    visited[sr][sc] = true;

    int dr[] = { -1, 1, 0, 0 };
    int dc[] = { 0, 0, -1, 1 };

    while (!stiva.empty()) {
        std::pair<int, int> top = stiva.back();
        stiva.pop_back();

        int r = top.first;
        int c = top.second;
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                grid[nr][nc] != 0 && !visited[nr][nc]) {
                visited[nr][nc] = true;
                stiva.push_back(std::make_pair(nr, nc));
            }
        }
    }
    int total = 0, visitedCount = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] != 0) {
                total++;
                if (visited[i][j]) visitedCount++;
            }
        }
    }

    return visitedCount == total;
}
