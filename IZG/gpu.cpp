/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
//xkromp00, dokoncene 22.5.2020, ocakvany pocet bodov z projektu: 15.7
using namespace std;

#include <student/gpu.hpp>
#include <list> // zoznam
#include <stdlib.h> //malloc
#include <cstring> //memcpy
#include <vector> 



struct Buffer {
    BufferID identifikator; //ID
    char* ukazatel; //malloc
    int velkost; //size
};

vector<Buffer> zoznam_Bufferov;
Buffer novy_buffer;
      
/// \addtogroup gpu_init
/// @{

/**
 * @brief Constructor of GPU
 */
GPU::GPU(){
  /// \todo Zde můžete alokovat/inicializovat potřebné proměnné grafické karty
}

/**
 * @brief Destructor of GPU
 */
GPU::~GPU(){
  /// \todo Zde můžete dealokovat/deinicializovat grafickou kartu

    deleteFramebuffer();

    for (int i = 0; i < zoznam_Bufferov.size(); i++) {
        deleteBuffer(zoznam_Bufferov[i].identifikator);
    }
}

/// @}

/** \addtogroup buffer_tasks 01. Implementace obslužných funkcí pro buffery
 * @{
 */

/**
 * @brief This function allocates buffer on GPU.
 *
 * @param size size in bytes of new buffer on GPU.
 *
 * @return unique identificator of the buffer
 */
BufferID GPU::createBuffer(uint64_t size) { 
  /// \todo Tato funkce by měla na grafické kartě vytvořit buffer dat.<br>
  /// Velikost bufferu je v parameteru size (v bajtech).<br>
  /// Funkce by měla vrátit unikátní identifikátor identifikátor bufferu.<br>
  /// Na grafické kartě by mělo být možné alkovat libovolné množství bufferů o libovolné velikosti.<br>
    BufferID nove_ID = 0; 
    
    novy_buffer.ukazatel = (char*) malloc(size);
    vector<Buffer>::iterator i;
    for (i = zoznam_Bufferov.begin(); i != zoznam_Bufferov.end(); i++) {
        if (i->identifikator >= nove_ID) {   //ked je nula nula tak sa nula neprepise na vacsie cislo
            nove_ID = i->identifikator + 1;
      }
    }
    novy_buffer.identifikator = nove_ID;
    novy_buffer.velkost = size;
    zoznam_Bufferov.push_back(novy_buffer);
  return nove_ID; 
}

/**
 * @brief This function frees allocated buffer on GPU.
 *
 * @param buffer buffer identificator
 */
void GPU::deleteBuffer(BufferID buffer) {
  /// \todo Tato funkce uvolní buffer na grafické kartě.
  /// Buffer pro smazání je vybrán identifikátorem v parameteru "buffer".
  /// Po uvolnění bufferu je identifikátor volný a může být znovu použit při vytvoření nového bufferu.

    vector<Buffer>::iterator i;
    for (i = zoznam_Bufferov.begin(); i != zoznam_Bufferov.end(); i++) {
        if ( i->identifikator == buffer) {
            free(i->ukazatel); //uvolnujem ukazatel
            zoznam_Bufferov.erase(i); //vymazavam cely prvok, nie len ukazatel
            break;
        }
    }
    
}

/**
 * @brief This function uploads data to selected buffer on the GPU
 *
 * @param buffer buffer identificator
 * @param offset specifies the offset into the buffer's data
 * @param size specifies the size of buffer that will be uploaded
 * @param data specifies a pointer to new data
 */
void GPU::setBufferData(BufferID buffer, uint64_t offset, uint64_t size, void const* data) {
  /// \todo Tato funkce nakopíruje data z cpu na "gpu".<br>
  /// Data by měla být nakopírována do bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje, kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) kam se data nakopírují.<br>
  /// Parametr data obsahuje ukazatel na data na cpu pro kopírování.<br>

  //  memcpy(&offset, &data, sizeof(size));

    vector<Buffer>::iterator i;
    for (i = zoznam_Bufferov.begin(); i != zoznam_Bufferov.end(); i++) {
        if (buffer == i->identifikator) {
           memcpy(i->ukazatel + offset, (char*)data , size);
        }
    }
    
}

/**
 * @brief This function downloads data from GPU.
 *
 * @param buffer specfies buffer
 * @param offset specifies the offset into the buffer from which data will be returned, measured in bytes. 
 * @param size specifies data size that will be copied
 * @param data specifies a pointer to the location where buffer data is returned. 
 */
void GPU::getBufferData(BufferID buffer,
                        uint64_t offset,
                        uint64_t size,
                        void*    data)
{
  /// \todo Tato funkce vykopíruje data z "gpu" na cpu.
  /// Data by měla být vykopírována z bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) odkud se začne kopírovat.<br>
  /// Parametr data obsahuje ukazatel, kam se data nakopírují.<br>
    vector<Buffer>::iterator i;
    for (i = zoznam_Bufferov.begin(); i != zoznam_Bufferov.end(); i++) {
        if (buffer == i->identifikator) {
            memcpy((char*)data ,  i->ukazatel + offset , size);
        }
    }
}

/**
 * @brief This function tests if buffer exists
 *
 * @param buffer selected buffer id
 *
 * @return true if buffer points to existing buffer on the GPU.
 */
bool GPU::isBuffer(BufferID buffer) { 
  /// \todo Tato funkce by měla vrátit true pokud buffer je identifikátor existující bufferu.<br>
  /// Tato funkce by měla vrátit false, pokud buffer není identifikátor existujícího bufferu. (nebo bufferu, který byl smazán).<br>
  /// Pro emptyId vrací false.<br>
    vector<Buffer>::iterator i;
    for (i = zoznam_Bufferov.begin(); i != zoznam_Bufferov.end(); i++) {
        if( buffer == i->identifikator)
        return true;
    }
  return false; 
}

/// @}

/**
 * \addtogroup vertexpuller_tasks 02. Implementace obslužných funkcí pro vertex puller
 * @{
 */

struct Indexing {
    bool enabled = false;
    BufferID identifikator;
    IndexType typ;
};

struct Head {
    BufferID identifikator;
    int offset;
    int stride;
    AttributeType typ;
    bool enabled = false;
};


struct VertexPuller {
    ObjectID identifikator; //ID
    Indexing indexing;
    Head hlavy[maxAttributes];
};


list<VertexPuller> zoznam_Vertexov;

VertexPuller *aktivny_puller = nullptr;

/**
 * @brief This function creates new vertex puller settings on the GPU,
 *
 * @return unique vertex puller identificator
 */
ObjectID GPU::createVertexPuller     (){
  /// \todo Tato funkce vytvoří novou práznou tabulku s nastavením pro vertex puller.<br>
  /// Funkce by měla vrátit identifikátor nové tabulky.
  /// Prázdná tabulka s nastavením neobsahuje indexování a všechny čtecí hlavy jsou vypnuté.

    ObjectID nove_ID = 0;
    VertexPuller novy_vertex;
    list<VertexPuller>::iterator it;
    for (it = zoznam_Vertexov.begin(); it != zoznam_Vertexov.end(); it++) {
        if (it->identifikator >= nove_ID) {   
            nove_ID = it->identifikator + 1;
        }
    }
    novy_vertex.identifikator = nove_ID;
    zoznam_Vertexov.push_back(novy_vertex);
    return nove_ID;

}

/**
 * @brief This function deletes vertex puller settings
 *
 * @param vao vertex puller identificator
 */
void     GPU::deleteVertexPuller     (VertexPullerID vao){
  /// \todo Tato funkce by měla odstranit tabulku s nastavení pro vertex puller.<br>
  /// Parameter "vao" obsahuje identifikátor tabulky s nastavením.<br>
  /// Po uvolnění nastavení je identifiktátor volný a může být znovu použit.<br>

    list<VertexPuller>::iterator it;
    for (it = zoznam_Vertexov.begin(); it != zoznam_Vertexov.end(); it++) {
        if (it->identifikator == vao) {
           zoznam_Vertexov.erase(it);
           return;
        }
    }
}

/**
 * @brief This function sets one vertex puller reading head.
 *
 * @param vao identificator of vertex puller
 * @param head id of vertex puller head
 * @param type type of attribute
 * @param stride stride in bytes
 * @param offset offset in bytes
 * @param buffer id of buffer
 */
void     GPU::setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer){
  /// \todo Tato funkce nastaví jednu čtecí hlavu vertex pulleru.<br>
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "head" vybírá čtecí hlavu vybraného vertex pulleru.<br>
  /// Parametr "type" nastaví typ atributu, který čtecí hlava čte. Tímto se vybere kolik dat v bajtech se přečte.<br>
  /// Parametr "stride" nastaví krok čtecí hlavy.<br>
  /// Parametr "offset" nastaví počáteční pozici čtecí hlavy.<br>
  /// Parametr "buffer" vybere buffer, ze kterého bude čtecí hlava číst.<br>

    list<VertexPuller>::iterator it;
    for (it = zoznam_Vertexov.begin(); it != zoznam_Vertexov.end(); it++) {
        if (it->identifikator == vao) {
            it->hlavy[head].identifikator = buffer;
            it->hlavy[head].typ = type;
            it->hlavy[head].stride = stride;
            it->hlavy[head].offset = offset;
            return;
        }
    }
    
}

/**
 * @brief This function sets vertex puller indexing.
 *
 * @param vao vertex puller id
 * @param type type of index
 * @param buffer buffer with indices
 */
void     GPU::setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer) {
  /// \todo Tato funkce nastaví indexování vertex pulleru.
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "type" volí typ indexu, který je uložený v bufferu.<br>
  /// Parametr "buffer" volí buffer, ve kterém jsou uloženy indexy.<br>

    list<VertexPuller>::iterator it;
    for (it = zoznam_Vertexov.begin(); it != zoznam_Vertexov.end(); it++) {
        if (it->identifikator == vao) {
            it->indexing.enabled = true;
            it->indexing.typ = type;
            it->indexing.identifikator = buffer;
            return;
        }
    }
}

/**
 * @brief This function enables vertex puller's head.
 *
 * @param vao vertex puller 
 * @param head head id
 */
void     GPU::enableVertexPullerHead (VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce povolí čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava povolena, hodnoty z bufferu se budou kopírovat do atributu vrcholů vertex shaderu.<br>
  /// Parametr "vao" volí tabulku s nastavením vertex pulleru (vybírá vertex puller).<br>
  /// Parametr "head" volí čtecí hlavu.<br>

    list<VertexPuller>::iterator it;
    for (it = zoznam_Vertexov.begin(); it != zoznam_Vertexov.end(); it++) {
        if (it->identifikator == vao) {
            it->hlavy[head].enabled = true;
            return;
        }
    }

}

/**
 * @brief This function disables vertex puller's head
 *
 * @param vao vertex puller id
 * @param head head id
 */
void     GPU::disableVertexPullerHead(VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce zakáže čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava zakázána, hodnoty z bufferu se nebudou kopírovat do atributu vrcholu.<br>
  /// Parametry "vao" a "head" vybírají vertex puller a čtecí hlavu.<br>


    list<VertexPuller>::iterator it;
    for (it = zoznam_Vertexov.begin(); it != zoznam_Vertexov.end(); it++) {
        if (it->identifikator == vao) {
            it->hlavy[head].enabled = false;
            return;
        }
    }
   
}

/**
 * @brief This function selects active vertex puller.
 *
 * @param vao id of vertex puller
 */
void     GPU::bindVertexPuller       (VertexPullerID vao){
  /// \todo Tato funkce aktivuje nastavení vertex pulleru.<br>
  /// Pokud je daný vertex puller aktivován, atributy z bufferů jsou vybírány na základě jeho nastavení.<br>
    list<VertexPuller>::iterator it;
    for (it = zoznam_Vertexov.begin(); it != zoznam_Vertexov.end(); it++) {
        if (it->identifikator == vao) {
            aktivny_puller = &(*it);
            return;
        }
    }

}

/**
 * @brief This function deactivates vertex puller.
 */
void     GPU::unbindVertexPuller     (){
  /// \todo Tato funkce deaktivuje vertex puller.
  /// To většinou znamená, že se vybere neexistující "emptyID" vertex puller.

    aktivny_puller = nullptr;
}

/**
 * @brief This function tests if vertex puller exists.
 *
 * @param vao vertex puller
 *
 * @return true, if vertex puller "vao" exists
 */
bool     GPU::isVertexPuller(VertexPullerID vao) {
    /// \todo Tato funkce otestuje, zda daný vertex puller existuje.
    /// Pokud ano, funkce vrací true.
    
    
    list<VertexPuller>::iterator it;
    for (it = zoznam_Vertexov.begin(); it != zoznam_Vertexov.end(); it++) {
        if (vao == it->identifikator) {
          return true;
        } 
    }
    return false;
}

/// @}

/** \addtogroup program_tasks 03. Implementace obslužných funkcí pro shader programy
 * @{
 */
struct Program
{
    ProgramID identifikator;
    VertexShader vertex_shader;
    FragmentShader fragment_shader;
    Uniforms uniforms;
    AttributeType typ[maxAttributes];
};

list<Program> zoznam_Programov;

Program *aktivny_program = nullptr;

/**
 * @brief This function creates new shader program.
 *
 * @return shader program id
 */
ProgramID        GPU::createProgram         (){
  /// \todo Tato funkce by měla vytvořit nový shader program.<br>
  /// Funkce vrací unikátní identifikátor nového proramu.<br>
  /// Program je seznam nastavení, které obsahuje: ukazatel na vertex a fragment shader.<br>
  /// Dále obsahuje uniformní proměnné a typ výstupních vertex attributů z vertex shaderu, které jsou použity pro interpolaci do fragment atributů.<br>

    ProgramID nove_ID = 0;
    Program novy_program;
    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (it->identifikator >= nove_ID) {
            nove_ID = it->identifikator + 1;
        }
    }
    novy_program.identifikator = nove_ID;
    zoznam_Programov.push_back(novy_program);
    return nove_ID;

}

/**
 * @brief This function deletes shader program
 *
 * @param prg shader program id
 */
void             GPU::deleteProgram         (ProgramID prg){
  /// \todo Tato funkce by měla smazat vybraný shader program.<br>
  /// Funkce smaže nastavení shader programu.<br>
  /// Identifikátor programu se stane volným a může být znovu využit.<br>
    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (it->identifikator == prg) {
            zoznam_Programov.erase(it);
            return;
        }
    }

}

/**
 * @brief This function attaches vertex and frament shader to shader program.
 *
 * @param prg shader program
 * @param vs vertex shader 
 * @param fs fragment shader
 */
void             GPU::attachShaders         (ProgramID prg,VertexShader vs,FragmentShader fs){
  /// \todo Tato funkce by měla připojít k vybranému shader programu vertex a fragment shader.
    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (it->identifikator == prg) {
            it->fragment_shader = fs;
            it->vertex_shader = vs;
            return;
        }
    }

}

/**
 * @brief This function selects which vertex attributes should be interpolated during rasterization into fragment attributes.
 *
 * @param prg shader program
 * @param attrib id of attribute
 * @param type type of attribute
 */
void             GPU::setVS2FSType          (ProgramID prg,uint32_t attrib,AttributeType type){
  /// \todo tato funkce by měla zvolit typ vertex atributu, který je posílán z vertex shaderu do fragment shaderu.<br>
  /// V průběhu rasterizace vznikají fragment.<br>
  /// Fragment obsahují fragment atributy.<br>
  /// Tyto atributy obsahují interpolované hodnoty vertex atributů.<br>
  /// Tato funkce vybere jakého typu jsou tyto interpolované atributy.<br>
  /// Bez jakéhokoliv nastavení jsou atributy prázdne AttributeType::EMPTY<br>

    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (it->identifikator == prg) {
            it->typ[attrib] = type;
            return;
        }
    }
}

/**
 * @brief This function actives selected shader program
 *
 * @param prg shader program id
 */
void             GPU::useProgram            (ProgramID prg){
  /// \todo tato funkce by měla vybrat aktivní shader program.

    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (it->identifikator == prg) {
            aktivny_program = &(*it);
            return;
        }
    }

}

/**
 * @brief This function tests if selected shader program exists.
 *
 * @param prg shader program
 *
 * @return true, if shader program "prg" exists.
 */
bool             GPU::isProgram             (ProgramID prg){
  /// \todo tato funkce by měla zjistit, zda daný program existuje.<br>
  /// Funkce vráti true, pokud program existuje.<br>

    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (prg == it->identifikator) {
            return true;
        }
    }
    return false;

}

/**
 * @brief This function sets uniform value (1 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform1f      (ProgramID prg,uint32_t uniformId,float     const&d){
  /// \todo tato funkce by měla nastavit uniformní proměnnou shader programu.<br>
  /// Parametr "prg" vybírá shader program.<br>
  /// Parametr "uniformId" vybírá uniformní proměnnou. Maximální počet uniformních proměnných je uložen v programné \link maxUniforms \endlink.<br>
  /// Parametr "d" obsahuje data (1 float).<br>

    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (prg == it->identifikator) {
            it->uniforms.uniform[uniformId].v1 = d;
        }
    }

}

/**
 * @brief This function sets uniform value (2 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform2f      (ProgramID prg,uint32_t uniformId,glm::vec2 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 2 floaty.
    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (prg == it->identifikator) {
            it->uniforms.uniform[uniformId].v2 = d;
        }
    }


}

/**
 * @brief This function sets uniform value (3 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform3f      (ProgramID prg,uint32_t uniformId,glm::vec3 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 3 floaty.
    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (prg == it->identifikator) {
            it->uniforms.uniform[uniformId].v3 = d;
        }
    }

}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform4f      (ProgramID prg,uint32_t uniformId,glm::vec4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 4 floaty.
    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (prg == it->identifikator) {
            it->uniforms.uniform[uniformId].v4 = d;
        }
    }

}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniformMatrix4f(ProgramID prg,uint32_t uniformId,glm::mat4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává matici 4x4 (16 floatů).
    list<Program>::iterator it;
    for (it = zoznam_Programov.begin(); it != zoznam_Programov.end(); it++) {
        if (prg == it->identifikator) {
            it->uniforms.uniform[uniformId].m4 = d;
        }
    }

}

/// @}





/** \addtogroup framebuffer_tasks 04. Implementace obslužných funkcí pro framebuffer
 * @{
 */

struct Framebuffer
{
    float* depth_buffer;
    uint8_t* color_buffer;
    int width;
    int height;
};

Framebuffer novy_framebuffer;

/**
 * @brief This function creates framebuffer on GPU.
 *
 * @param width width of framebuffer
 * @param height height of framebuffer
 */
void GPU::createFramebuffer      (uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla alokovat framebuffer od daném rozlišení.<br>
  /// Framebuffer se skládá z barevného a hloukového bufferu.<br>
  /// Buffery obsahují width x height pixelů.<br>
  /// Barevný pixel je složen z 4 x uint8_t hodnot - to reprezentuje RGBA barvu.<br>
  /// Hloubkový pixel obsahuje 1 x float - to reprezentuje hloubku.<br>
  /// Nultý pixel framebufferu je vlevo dole.<br>

    novy_framebuffer.height = height;
    novy_framebuffer.width = width;
    novy_framebuffer.color_buffer = (uint8_t*) malloc(4 * width * height * sizeof(uint8_t));
    novy_framebuffer.depth_buffer = (float*) malloc(height * width * sizeof(float));
}

/**
 * @brief This function deletes framebuffer.
 */
void GPU::deleteFramebuffer      (){
  /// \todo tato funkce by měla dealokovat framebuffer.
    if (novy_framebuffer.color_buffer != nullptr) {
        free(novy_framebuffer.color_buffer);
        novy_framebuffer.color_buffer = nullptr;
    }
    if (novy_framebuffer.depth_buffer != nullptr) {
        free(novy_framebuffer.depth_buffer);
        novy_framebuffer.depth_buffer = nullptr;
    }

    novy_framebuffer.height = 0;
    novy_framebuffer.width = 0;
}

/**
 * @brief This function resizes framebuffer.
 *
 * @param width new width of framebuffer
 * @param height new heght of framebuffer
 */
void     GPU::resizeFramebuffer(uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla změnit velikost framebuffer.
    novy_framebuffer.height = height;
    novy_framebuffer.width = width;
    
}

/**
 * @brief This function returns pointer to color buffer.
 *
 * @return pointer to color buffer
 */
uint8_t* GPU::getFramebufferColor  (){
  /// \todo Tato funkce by měla vrátit ukazatel na začátek barevného bufferu.<br>
    uint8_t* ukazatel = novy_framebuffer.color_buffer;
    return ukazatel;
}

/**
 * @brief This function returns pointer to depth buffer.
 *
 * @return pointer to dept buffer.
 */
float* GPU::getFramebufferDepth    (){
  /// \todo tato funkce by mla vrátit ukazatel na začátek hloubkového bufferu.<br>
    
    float* ukazatel = novy_framebuffer.depth_buffer;
    return ukazatel;
 // return nullptr;
}

/**
 * @brief This function returns width of framebuffer
 *
 * @return width of framebuffer
 */
uint32_t GPU::getFramebufferWidth (){
  /// \todo Tato funkce by měla vrátit šířku framebufferu.

  return novy_framebuffer.width;
}

/**
 * @brief This function returns height of framebuffer.
 *
 * @return height of framebuffer
 */
uint32_t GPU::getFramebufferHeight(){
  /// \todo Tato funkce by měla vrátit výšku framebufferu.
  return novy_framebuffer.height;
}

/// @}

/** \addtogroup draw_tasks 05. Implementace vykreslovacích funkcí
 * Bližší informace jsou uvedeny na hlavní stránce dokumentace.
 * @{
 */

/**
 * @brief This functino clears framebuffer.
 *
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void            GPU::clear                 (float r,float g,float b,float a){
  /// \todo Tato funkce by měla vyčistit framebuffer.<br>
  /// Barevný buffer vyčistí na barvu podle parametrů r g b a (0 - nulová intenzita, 1 a větší - maximální intenzita).<br>
  /// (0,0,0) - černá barva, (1,1,1) - bílá barva.<br>
  /// Hloubkový buffer nastaví na takovou hodnotu, která umožní rasterizaci trojúhelníka, který leží v rámci pohledového tělesa.<br>
  /// Hloubka by měla být tedy větší než maximální hloubka v NDC (normalized device coordinates).<br>

    for (int i = 0; i < novy_framebuffer.width * novy_framebuffer.height; i++) {
        uint8_t red = (uint8_t)r * 255;
        uint8_t green = (uint8_t)g * 255;
        uint8_t blue = (uint8_t)b * 255;
        uint8_t alpha = (uint8_t)a * 255;

        memcpy(novy_framebuffer.color_buffer + i * 4, &red, 1);
        memcpy(novy_framebuffer.color_buffer + i * 4 + 1, &green, 1);
        memcpy(novy_framebuffer.color_buffer + i * 4 + 2, &blue, 1);
        memcpy(novy_framebuffer.color_buffer + i * 4 + 3, &alpha, 1);

        float velkyFloat = 100;

        memcpy(novy_framebuffer.depth_buffer, &velkyFloat, sizeof(float));
    }
}



void            GPU::drawTriangles(uint32_t  nofVertices) {
    /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
    /// Vrcholy se budou vybírat podle nastavení z aktivního vertex pulleru (pomocí bindVertexPuller).<br>
    /// Vertex shader a fragment shader se zvolí podle aktivního shader programu (pomocí useProgram).<br>
    /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>

    // vytvorit zoznam outvertexov

    VertexPuller puller = *aktivny_puller;
    

    for (int i = 0; i < nofVertices; i++) { //prechadzam kazdy vertex
        InVertex inVertex;
        // nepouzivam indexovanie a teda id vertexu je take iste ako poradie jeho spracovania
        if (aktivny_puller->indexing.enabled == false) {
            inVertex.gl_VertexID = i;
            
        }
        // pouzivam indexovanie a teda
        else {
            
            if (aktivny_puller->indexing.typ == IndexType::UINT8) {
                uint8_t id;
                getBufferData(aktivny_puller->indexing.identifikator, i, 1, &id);
                inVertex.gl_VertexID = id;
            }
            else if (aktivny_puller->indexing.typ == IndexType::UINT16)
            {
                uint16_t id;
                getBufferData(aktivny_puller->indexing.identifikator, i * 2, 2, &id);
                inVertex.gl_VertexID = id;
            }
            else if (aktivny_puller->indexing.typ == IndexType::UINT32)
            {
                uint32_t id;
                getBufferData(aktivny_puller->indexing.identifikator, i * 4 , 4, &id);
                inVertex.gl_VertexID = id;
            }
        }
        
        for (int j = 0; j < maxAttributes; j++) { //kazdy zo 16stich atributov
            Head aktualnaHlava = aktivny_puller->hlavy[j]; //skratka
            if (aktualnaHlava.enabled == false) continue;
            
            BufferID buffer = aktualnaHlava.identifikator;

            float f1, f2, f3, f4;

            if (aktualnaHlava.typ == AttributeType::FLOAT) { //podla typu atributu citam z pamati
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID, sizeof(aktualnaHlava.typ), &f1);
                inVertex.attributes[j].v1 = f1;
            }
            else if (aktualnaHlava.typ == AttributeType::VEC2) {
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID, sizeof(aktualnaHlava.typ), &f1);
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID + sizeof(float), sizeof(aktualnaHlava.typ), &f2);
                
                inVertex.attributes[j].v2 = glm::vec2(f1, f2);
            }
            else if (aktualnaHlava.typ == AttributeType::VEC3) {
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID, sizeof(aktualnaHlava.typ), &f1);
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID + sizeof(float), sizeof(aktualnaHlava.typ), &f2);
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID + sizeof(float) * 2, sizeof(aktualnaHlava.typ), &f3);

                inVertex.attributes[j].v3 = glm::vec3(f1, f2, f3);
            }
            else if (aktualnaHlava.typ == AttributeType::VEC4) {
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID, sizeof(aktualnaHlava.typ), &f1);
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID + sizeof(float), sizeof(aktualnaHlava.typ), &f2);
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID + sizeof(float) * 2, sizeof(aktualnaHlava.typ), &f3);
                getBufferData(buffer, aktualnaHlava.offset + aktualnaHlava.stride * inVertex.gl_VertexID + sizeof(float) * 3, sizeof(aktualnaHlava.typ), &f4);

                inVertex.attributes[j].v4 = glm::vec4(f1, f2, f3, f4);
            }
        }

        OutVertex outVertex;
        aktivny_program->vertex_shader(outVertex, inVertex, aktivny_program->uniforms); //kazdy vertex posielam cez vertex shader

        // pridat do zoznamu vertexov
    }
    

    //free(i->ukazatel);
}

/// @}
