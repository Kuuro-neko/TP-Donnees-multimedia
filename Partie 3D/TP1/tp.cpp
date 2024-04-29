// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"

#define MAX_SHAPES 6
#define DEFAULT_N 20
#define DEFAULT_PYRAMID_DEPTH 3


using namespace std;


enum DisplayMode{ WIRE=0, SOLID=1, LIGHTED_WIRE=2, LIGHTED=3 };

struct Triangle {
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline Triangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    unsigned int & operator [] (unsigned int iv) { return v[iv]; }
    unsigned int operator [] (unsigned int iv) const { return v[iv]; }
    inline virtual ~Triangle () {}
    inline Triangle & operator = (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};

struct Mesh {
    std::vector< Vec3 > vertices;
    std::vector< Vec3 > normals;
    std::vector< Triangle > triangles;
};

struct Pyramide { // Pour la fractale de Sierpinski
    inline Pyramide (std::vector<Vec3> vertices) {
        vbase0 = vertices[0];
        vbase1 = vertices[1];
        vbase2 = vertices[2];
        vbase3 = vertices[3];
        vtop = vertices[4];
    }
    void sendToMesh(Mesh & mesh, int offset) {
        mesh.vertices.push_back(vbase0);
        mesh.vertices.push_back(vbase1);
        mesh.vertices.push_back(vbase2);
        mesh.vertices.push_back(vbase3);
        mesh.vertices.push_back(vtop);

        mesh.normals.push_back(vbase0);
        mesh.normals.push_back(vbase1);
        mesh.normals.push_back(vbase2);
        mesh.normals.push_back(vbase3);
        mesh.normals.push_back(vtop);

        mesh.triangles.push_back(Triangle(offset+0, offset+1, offset+2));
        mesh.triangles.push_back(Triangle(offset+1, offset+3, offset+2));
        mesh.triangles.push_back(Triangle(offset+0, offset+2, offset+4));
        mesh.triangles.push_back(Triangle(offset+2, offset+3, offset+4));
        mesh.triangles.push_back(Triangle(offset+3, offset+1, offset+4));
        mesh.triangles.push_back(Triangle(offset+1, offset+0, offset+4));
    }
    std::vector<Pyramide> getDivisions() {
        std::vector<Pyramide> divisions;
        Vec3 v01 = (vbase0+vbase1)/2;
        Vec3 v02 = (vbase0+vbase2)/2;
        Vec3 v13 = (vbase1+vbase3)/2;
        Vec3 v23 = (vbase2+vbase3)/2;
        Vec3 vm = (vbase0+vbase1+vbase2+vbase3)/4;
        Vec3 v04 = (vbase0+vtop)/2;
        Vec3 v14 = (vbase1+vtop)/2;
        Vec3 v24 = (vbase2+vtop)/2;
        Vec3 v34 = (vbase3+vtop)/2;
        divisions.push_back(Pyramide({vbase0, v01, v02, vm, v04}));
        divisions.push_back(Pyramide({v01, vbase1, vm, v13, v14}));
        divisions.push_back(Pyramide({v02, vm, vbase2, v23, v24}));
        divisions.push_back(Pyramide({vm, v13, v23, vbase3, v34}));
        divisions.push_back(Pyramide({v04, v14, v24, v34, vtop}));
        return divisions;
    }
    // membres :
    Vec3 vbase0;
    Vec3 vbase1;
    Vec3 vbase2;
    Vec3 vbase3;
    Vec3 vtop;
};

Mesh mesh;

//Mesh to generate
Mesh unit_sphere;
Mesh additionnal_shape;

bool display_normals;
bool display_loaded_mesh;
bool display_unit_sphere;
bool display_additionnal;
DisplayMode displayMode;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;
static int n=DEFAULT_N;
static int pyramidDepth=DEFAULT_PYRAMID_DEPTH;
static int bypassN = 0;
static int shape = 3;

void setUnitSphere( Mesh & o_mesh, int nX=20, int nY=20 )
{
    o_mesh.vertices.clear();
    o_mesh.triangles.clear();
    o_mesh.normals.clear();
    for (int i = 0; i <= nX; i++) {
        for (int j = 0; j <= nY; j++) {
            double x = cos(2*M_PI * i/nX)*cos(M_PI * j/nY - M_PI_2);
            double y = sin(2*M_PI * i/nX)*cos(M_PI * j/nY - M_PI_2);
            double z = sin(M_PI * j/nY - M_PI_2);
            o_mesh.vertices.push_back( Vec3(x, y, z) );
            o_mesh.normals.push_back( Vec3(x, y, z) );
        }
    }

    for (unsigned int i = 0; i < o_mesh.vertices.size()-nX-1; i++) {
        o_mesh.triangles.push_back( Triangle(i, i+nX, i+nX+1));
        o_mesh.triangles.push_back( Triangle(i, i+nX+1, i+1));
    }
}

void sierpinskiPyramid(std::vector<Pyramide> & pyramides, int depth, int progress=1) {
    std::vector<Pyramide> divisions;
    for (long unsigned int i = 0; i < pyramides.size(); i++) {
        Pyramide p = pyramides[i];
        for (Pyramide division : p.getDivisions()) {
            divisions.push_back(division);
        }
    }
    pyramides.clear();
    for (Pyramide p : divisions) {
        pyramides.push_back(p);
    }
    if (progress < depth) {
        sierpinskiPyramid(pyramides, depth, progress+1);
    }
}

void setAdditionnalMesh( Mesh & o_mesh, int shape=0 )
{
    int nX, nY;
    if (shape == 3) {
        nX = pyramidDepth;
        nY = pyramidDepth;
    } else {
        nX = n;
        nY = n;
    }
    o_mesh.vertices.clear();
    o_mesh.triangles.clear();
    o_mesh.normals.clear();

    switch (shape) {
    case 0: // Cone
    {
        o_mesh.vertices.push_back( Vec3(0, 0, -0.5) );
        o_mesh.vertices.push_back( Vec3(0, 0, 1) );

        o_mesh.normals.push_back( Vec3(0,0, -0.5) );
        o_mesh.normals.push_back( Vec3(0,0, 1) );

        for (int i = 0; i <= nX; i++) {
            double x = cos(2*M_PI * i/nX);
            double y = sin(2*M_PI * i/nX);
            double z = -0.5;
            o_mesh.vertices.push_back( Vec3(x, y, z) );
            o_mesh.normals.push_back( Vec3(x, y, z) );
        }

        for (unsigned int i = 2; i < o_mesh.vertices.size()-1; i++) {
            o_mesh.triangles.push_back( Triangle(i, 0, i+1));
            o_mesh.triangles.push_back( Triangle(i, i+1, 1));
        }
        break;
    }
    case 1: // Diabolo
    {
        if (nY%2 == 1) {
            nY++;
        }

        // Premiere demi sphere

        for (int i = 0; i <= nX; i++) {
            for (int j = 0; j <= nY/2; j++) {
                double x = cos(2*M_PI * i/nX)*cos(M_PI * j/nY - M_PI_2);
                double y = sin(2*M_PI * i/nX)*cos(M_PI * j/nY - M_PI_2);
                double z = sin(M_PI * j/nY - M_PI_2) + 1;
                o_mesh.vertices.push_back( Vec3(x, y, z) );
                o_mesh.normals.push_back( Vec3(x, y, z) );
            }
        }

        o_mesh.vertices.push_back( Vec3(0, 0, 1) );

        for (int i = 0; i < nX; i++) {
            for (int j = 0; j < nY/2; j++) {
                o_mesh.triangles.push_back( Triangle(i*(nY/2+1)+j, (i+1)*(nY/2+1)+j, i*(nY/2+1)+j+1));
                o_mesh.triangles.push_back( Triangle(i*(nY/2+1)+j+1, (i+1)*(nY/2+1)+j, (i+1)*(nY/2+1)+j+1));
                if (j == nY/2-1) {
                    o_mesh.triangles.push_back( Triangle(i*(nY/2+1)+j+1, (i+1)*(nY/2+1)+j+1, o_mesh.vertices.size()-1));
                }
            }
        }

        int offset = o_mesh.vertices.size();

        // Deuxieme demi sphere

        for (int i = nX; i >= 0; i--) {
            for (int j = nY; j>= nY/2 ;j--) {
                double x = cos(2*M_PI * i/nX)*cos(M_PI * j/nY - M_PI_2);
                double y = sin(2*M_PI * i/nX)*cos(M_PI * j/nY - M_PI_2);
                double z = sin(M_PI * j/nY - M_PI_2) - 1;
                o_mesh.vertices.push_back( Vec3(x, y, z) );
                o_mesh.normals.push_back( Vec3(x, y, z) );
            }   
        }

        o_mesh.vertices.push_back( Vec3(0, 0, -1) );

        for (int i = 0; i < nX; i++) {
            for (int j = 0; j < nY/2; j++) {
                o_mesh.triangles.push_back( Triangle(offset+i*(nY/2+1)+j, offset+(i+1)*(nY/2+1)+j, offset+i*(nY/2+1)+j+1));
                o_mesh.triangles.push_back( Triangle(offset+i*(nY/2+1)+j+1, offset+(i+1)*(nY/2+1)+j, offset+(i+1)*(nY/2+1)+j+1));
                if (j == nY/2-1) {
                    o_mesh.triangles.push_back( Triangle(offset+i*(nY/2+1)+j+1, offset+(i+1)*(nY/2+1)+j+1, o_mesh.vertices.size()-1));
                }
            }
        }
        break;
    }
    case 2: // Dodecahedron
    {
        double r = 1;
        // Cube vertices
        for (int x = -1; x <= 1; x+=2) {
            for (int y = -1; y <= 1; y+=2) {
                for (int z = -1; z <= 1; z+=2) {
                    o_mesh.vertices.push_back( Vec3(x*r/sqrt(3), y*r/sqrt(3), z*r/sqrt(3)) );
                    o_mesh.normals.push_back( Vec3(x*r/sqrt(3), y*r/sqrt(3), z*r/sqrt(3)) );
                }
            }
        }

        // Dodecahedron vertices
        double phi = (1+sqrt(5))/2;
        for (int diffx = -1; diffx <= 1; diffx+=2) {
            for (int diffy = -1; diffy <= 1; diffy+=2) {
                o_mesh.vertices.push_back( Vec3(0, diffx*r/(sqrt(3)*phi), diffy*(r*phi)/sqrt(3)) );
                o_mesh.normals.push_back( Vec3(0, diffx*r/(sqrt(3)*phi), diffy*(r*phi)/sqrt(3)) );
                
            }
        }
        for (int diffx = -1; diffx <= 1; diffx+=2) {
            for (int diffy = -1; diffy <= 1; diffy+=2) {
                o_mesh.vertices.push_back( Vec3(diffx*r/(sqrt(3)*phi), diffy*(r*phi)/sqrt(3), 0) );
                o_mesh.normals.push_back( Vec3(diffx*r/(sqrt(3)*phi), diffy*(r*phi)/sqrt(3), 0) );
                
            }
        }
        for (int diffx = -1; diffx <= 1; diffx+=2) {
            for (int diffy = -1; diffy <= 1; diffy+=2) {
                o_mesh.vertices.push_back( Vec3(diffy*(r*phi)/sqrt(3), 0, diffx*r/(sqrt(3)*phi)) );
                o_mesh.normals.push_back( Vec3(diffy*(r*phi)/sqrt(3), 0, diffx*r/(sqrt(3)*phi)) );
                
            }
        }

        int faces[12*5] = {
            0, 12, 1, 18, 16,
            0, 16, 2, 10, 8,
            0, 8, 4, 14, 12,
            1, 18, 3, 11, 9,
            1, 9, 5, 14, 12,
            2, 10, 6, 15, 13,
            2, 13, 3, 18, 16,
            3, 11, 7, 15, 13,
            4, 14, 5, 19, 17,
            4, 17, 6, 10, 8,
            5, 19, 7, 11, 9,
            6, 15, 7, 19, 17
        };

        for (int i = 0; i < 12*5; i+=5) {
            o_mesh.triangles.push_back( Triangle(faces[i], faces[i+1], faces[i+4]) );
            o_mesh.triangles.push_back( Triangle(faces[i+1], faces[i+3], faces[i+4]) );
            o_mesh.triangles.push_back( Triangle(faces[i+3], faces[i+1], faces[i+2]) );
            o_mesh.triangles.push_back( Triangle(faces[i], faces[i+4], faces[i+1]) );
            o_mesh.triangles.push_back( Triangle(faces[i+1], faces[i+4], faces[i+3]) );
            o_mesh.triangles.push_back( Triangle(faces[i+3], faces[i+2], faces[i+1]) );
        }   
        break;
    }
    case 3: // Sierpinski pyramid
    {
        std::vector<Pyramide> divisions;
        divisions.push_back(Pyramide({Vec3(-1, -1, -1), Vec3(1, -1, -1), Vec3(-1, -1, 1), Vec3(1, -1, 1), Vec3(0, 1, 0)}));

        sierpinskiPyramid(divisions, nX);
        int offset = 0;
        for (Pyramide p : divisions) {
            p.sendToMesh(o_mesh, offset);
            offset += 5;
        }
        break;
    }
    case 4: // Cylindre
    {
        // Cercles de base
        for (int i = 0; i <= nX; i++) {
            double x = cos(2*M_PI * i/nX);
            double y = sin(2*M_PI * i/nX);
            double z = 0.5;
            o_mesh.vertices.push_back( Vec3(x, y, z) );
            o_mesh.normals.push_back( Vec3(x, y, z) );
        }
        for (int i = 0; i <= nX; i++) {
            double x = cos(2*M_PI * i/nX);
            double y = sin(2*M_PI * i/nX);
            double z = -0.5;
            o_mesh.vertices.push_back( Vec3(x, y, z) );
            o_mesh.normals.push_back( Vec3(x, y, z) );
        }

        // Centre des cercles
        o_mesh.vertices.push_back( Vec3(0, 0, 0.5) );
        o_mesh.vertices.push_back( Vec3(0, 0, -0.5) );

        o_mesh.normals.push_back( Vec3(0,0, 0.5) );
        o_mesh.normals.push_back( Vec3(0,0, -0.5) );

        // Triangles des cercles de base
        for (unsigned int i = 0; i < nX; i++) {
            o_mesh.triangles.push_back( Triangle(i, (i%nX)+1, o_mesh.vertices.size()-2));
            o_mesh.triangles.push_back( Triangle(i+nX+1, o_mesh.vertices.size()-1, (i%nX)+1+nX+1));
        }

        // Triangles des parois
        for (unsigned int i = 0; i < nX; i++) {
            o_mesh.triangles.push_back( Triangle(i, i+nX+1, (i%nX)+1));
            o_mesh.triangles.push_back( Triangle(i+nX+1, (i%nX)+1+nX+1, (i%nX)+1));
        }
        break;
    }
    case 5: // Donut
    {
        double r = 0.5;
        double R = 1;
        for (int u = 0; u <= nX; u++) {
            for (int v = 0; v <= nY; v++) {
                double x = (R + r*cos(2*M_PI * v/nY))*cos(2*M_PI * u/nX);
                double y = (R + r*cos(2*M_PI * v/nY))*sin(2*M_PI * u/nX);
                double z = r*sin(2*M_PI * v/nY);

                o_mesh.vertices.push_back( Vec3(x, y, z) );
                o_mesh.normals.push_back( Vec3(x, y, z) );
            }
        }

        for (unsigned int i = 0; i < o_mesh.vertices.size()-nX-1; i++) {
            o_mesh.triangles.push_back( Triangle(i, i+nX, i+nX+1));
            o_mesh.triangles.push_back( Triangle(i, i+nX+1, i+1));
        }
        break;
    }
    }
}


bool saveOFF( const std::string & filename ,
              std::vector< Vec3 > & i_vertices ,
              std::vector< Vec3 > & i_normals ,
              std::vector< Triangle > & i_triangles,
              bool save_normals = true ) {
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open()) {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl ;

    unsigned int n_vertices = i_vertices.size() , n_triangles = i_triangles.size();
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for( unsigned int v = 0 ; v < n_vertices ; ++v ) {
        myfile << i_vertices[v][0] << " " << i_vertices[v][1] << " " << i_vertices[v][2] << " ";
        if (save_normals) myfile << i_normals[v][0] << " " << i_normals[v][1] << " " << i_normals[v][2] << std::endl;
        else myfile << std::endl;
    }
    for( unsigned int f = 0 ; f < n_triangles ; ++f ) {
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF( std::string const & filename,
              std::vector<Vec3> & o_vertices,
              std::vector<Vec3> & o_normals,
              std::vector< Triangle > & o_triangles,
              bool load_normals = true )
{
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;

    myfile >> magic_s;

    if( magic_s != "OFF" )
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices , n_faces , dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    o_vertices.clear();
    o_normals.clear();

    for( int v = 0 ; v < n_vertices ; ++v )
    {
        float x , y , z ;

        myfile >> x >> y >> z ;
        o_vertices.push_back( Vec3( x , y , z ) );

        if( load_normals ) {
            myfile >> x >> y >> z;
            o_normals.push_back( Vec3( x , y , z ) );
        }
    }

    o_triangles.clear();
    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if( n_vertices_on_face == 3 )
        {
            unsigned int _v1 , _v2 , _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle( _v1, _v2, _v3 ));
        }
        else if( n_vertices_on_face == 4 )
        {
            unsigned int _v1 , _v2 , _v3 , _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3 ));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }

}


// ------------------------------------

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);

    displayMode = LIGHTED;
    display_normals = false;
    display_unit_sphere = false;
    display_additionnal = false;
    display_loaded_mesh = true;

    glLineWidth(1.);
    glPointSize(4.);
}




// ------------------------------------
// rendering.
// ------------------------------------


void drawVector( Vec3 const & i_from, Vec3 const & i_to ) {

    glBegin(GL_LINES);
    glVertex3f( i_from[0] , i_from[1] , i_from[2] );
    glVertex3f( i_to[0] , i_to[1] , i_to[2] );
    glEnd();
}

void drawVertices( Mesh const & i_mesh ) {
    glBegin(GL_POINTS); //Fonction OpenGL de dessin de points
    for(unsigned int vIt = 0 ; vIt < i_mesh.vertices.size(); ++vIt) {
        Vec3 p = i_mesh.vertices[vIt];

        glVertex3f( p[0] , p[1] , p[2] );
    }
    glEnd();
}


void drawTriangleMesh( Mesh const & i_mesh ) {

    if( i_mesh.triangles.size() > 0 ){
        if( i_mesh.normals.size() > 0 ){
            //Fonction de dessin en utilisant les normales au sommet
            glBegin(GL_TRIANGLES); //Fonction OpenGL de dessin de triangles
            for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {
                Vec3 p0 = i_mesh.vertices[i_mesh.triangles[tIt][0]];
                Vec3 n0 = i_mesh.normals[i_mesh.triangles[tIt][0]];

                Vec3 p1 = i_mesh.vertices[i_mesh.triangles[tIt][1]];
                Vec3 n1 = i_mesh.normals[i_mesh.triangles[tIt][1]];

                Vec3 p2 = i_mesh.vertices[i_mesh.triangles[tIt][2]];
                Vec3 n2 = i_mesh.normals[i_mesh.triangles[tIt][2]];

                glNormal3f( n0[0] , n0[1] , n0[2] );
                glVertex3f( p0[0] , p0[1] , p0[2] );
                glNormal3f( n1[0] , n1[1] , n1[2] );
                glVertex3f( p1[0] , p1[1] , p1[2] );
                glNormal3f( n2[0] , n2[1] , n2[2] );
                glVertex3f( p2[0] , p2[1] , p2[2] );
            }
            glEnd();
        } else {
            //Fonction de dessin en utilisant sans normales
            glBegin(GL_TRIANGLES); //Fonction OpenGL de dessin de triangles
            for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {
                Vec3 p0 = i_mesh.vertices[i_mesh.triangles[tIt][0]];
                Vec3 p1 = i_mesh.vertices[i_mesh.triangles[tIt][1]];
                Vec3 p2 = i_mesh.vertices[i_mesh.triangles[tIt][2]];

                //Dessin des trois sommets formant le triangle
                glVertex3f( p0[0] , p0[1] , p0[2] );
                glVertex3f( p1[0] , p1[1] , p1[2] );
                glVertex3f( p2[0] , p2[1] , p2[2] );
            }
            glEnd();
        }
    } else {
        drawVertices(i_mesh);
    }

    if(display_normals){

        glColor3f(1.,0.,0.);
        for(unsigned int pIt = 0 ; pIt < i_mesh.normals.size() ; ++pIt) {
            Vec3 to = i_mesh.vertices[pIt] + 0.02*i_mesh.normals[pIt];
            drawVector(i_mesh.vertices[pIt], to);
        }

    }

}



void draw () {

    if(displayMode == LIGHTED || displayMode == LIGHTED_WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);

    }  else if(displayMode == WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glDisable (GL_LIGHTING);

    }  else if(displayMode == SOLID ){
        glDisable (GL_LIGHTING);
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    }

    if( display_additionnal ){
        glColor3f(1,0.8,0.8);
        drawTriangleMesh(additionnal_shape);
    }

    if( display_unit_sphere ){
        glColor3f(0.8,1,0.8);
        drawTriangleMesh(unit_sphere);
    }

    if( display_loaded_mesh ){
        glColor3f(0.8,0.8,1);
        drawTriangleMesh(mesh);
    }

    if(displayMode == SOLID || displayMode == LIGHTED_WIRE){
        glEnable (GL_POLYGON_OFFSET_LINE);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth (1.0f);
        glPolygonOffset (-2.0, 1.0);

        glColor3f(0.,0.,0.);
        if( display_unit_sphere )
            drawTriangleMesh(unit_sphere);

        if( display_loaded_mesh )
            drawTriangleMesh(mesh);

        if( display_additionnal )
            drawTriangleMesh(additionnal_shape);

        glDisable (GL_POLYGON_OFFSET_LINE);
        glEnable (GL_LIGHTING);
    }



}

void changeDisplayMode(){
    if(displayMode == LIGHTED)
        displayMode = LIGHTED_WIRE;
    else if(displayMode == LIGHTED_WIRE)
        displayMode = SOLID;
    else if(displayMode == SOLID)
        displayMode = WIRE;
    else
        displayMode = LIGHTED;
}

void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}

void printUsage () {
    cout << endl
         << "--------------------------------------" << endl
         << "TP1" << endl
         << "--------------------------------------" << endl
         << "USAGE: ./Main" << endl
         << "--------------------------------------" << endl
         << "Keyboard commands" << endl
         << "--------------------------------------" << endl
         << " ?: Print help" << endl
         << " n: Toggle normal display" << endl
         << " w: Toggle display mode" << endl
         << " 1: Toggle loaded mesh display" << endl
         << " 2: Toggle unit sphere mesh display" << endl
         << " 3: Toggle additionnal mesh display" << endl
         << " -: Decrease the number of meridians and parallels of the generated sphere or the Sierpinski Pyramid's depth" << endl
         << " +: Increase the number of meridians and parallels of the generated sphere or the Sierpinski Pyramid's depth" << endl
         << " r: Reset the number of meridians and parallels of the generated sphere and the Sierpinski Pyramid's depth" << endl
         << " S/s: Cycle through the additional mesh shapes" << endl
         << " f: Toggle full screen" << endl
         << " <drag>+<left button>: rotate the scene" << endl
         << " <drag>+<right button>: move the scene" << endl
         << " <drag>+<middle button>: zoom" << endl
         << "--------------------------------------" << endl;
}

void printAdditionnalShape(int shape) {
    switch (shape) {
        case 0:
            cout << "Additionnal shape (toggle it with 3) is now cone" << endl;
            break;
        case 1:
            cout << "Additionnal shape (toggle it with 3) is now diabolo" << endl;
            break;
        case 2:
            cout << "Additionnal shape (toggle it with 3) is now dodecahedron" << endl;
            break;
        case 3:
            cout << "Additionnal shape (toggle it with 3) is now Sierpinski pyramid" << endl;
            break;
        case 4:
            cout << "Additionnal shape (toggle it with 3) is now cylinder" << endl;
            break;
        case 5:
            cout << "Additionnal shape (toggle it with 3) is now donut" << endl;
            break;
        }
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;


    case 'w': //Change le mode d'affichage
        changeDisplayMode();
        break;


    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;

    case '1': //Toggle loaded mesh display
        display_loaded_mesh = !display_loaded_mesh;
        break;

    case '2': //Toggle unit sphere mesh display
        display_unit_sphere = !display_unit_sphere;
        if (display_unit_sphere) setUnitSphere( unit_sphere, n, n );
        break;

    case '3': //Toggle additionnal mesh display
        display_additionnal = !display_additionnal;
        if (display_additionnal) setAdditionnalMesh( additionnal_shape, shape );
        break;
    
    case '-':
        if ((shape != 3 || !display_additionnal || display_unit_sphere) && n > 2) n--;
        if (shape == 3 && pyramidDepth > 1 && display_additionnal) pyramidDepth--;
        if (display_unit_sphere) setUnitSphere( unit_sphere, n, n );
        if (display_additionnal) setAdditionnalMesh( additionnal_shape, shape );
        break;

    case '+':
        if(shape != 3 || !display_additionnal || display_unit_sphere) n++;
        if (shape == 3 && display_additionnal) {
            if (bypassN == 1 || pyramidDepth < 8) {
                pyramidDepth++;
            } else {
                cout << endl
                    << "-------- \033[31mWARNING\033[0m -------" << endl
                    << "Raising the depth of the pyramid to a higher value will result in a significant performance cost" << endl
                    << "Use <b> to toggle the limit" << endl
                    << "------------------------" << endl;
            }
        }
        if (display_unit_sphere) setUnitSphere( unit_sphere, n, n );
        if (display_additionnal) setAdditionnalMesh( additionnal_shape, shape );
        break;
    
    case 'S': //Cycle through the additional mesh shape
        shape = (shape==0)?MAX_SHAPES-1:shape-1;
        printAdditionnalShape(shape);
        setAdditionnalMesh( additionnal_shape, shape );
        break;

    case 's': //Cycle through the additional mesh shape
        shape = (shape+1)%MAX_SHAPES;
        printAdditionnalShape(shape);
        setAdditionnalMesh( additionnal_shape, shape );
        break;
    
    case 'b':
        bypassN = (bypassN+1)%2;
        if (bypassN == 1) {
            cout << "Bypassing the limit of the pyramid depth" << endl;
        } else {
            pyramidDepth = DEFAULT_PYRAMID_DEPTH;
            cout << "Limiting the depth of the pyramid" << endl;
        }
        break;
    
    case 'r':
        n = DEFAULT_N;
        pyramidDepth = DEFAULT_PYRAMID_DEPTH;
        if (display_unit_sphere) setUnitSphere( unit_sphere, n, n );
        if (display_additionnal) setAdditionnalMesh( additionnal_shape, shape );
        break;

    case '?':
    default:
        printUsage ();
        break;
    }
    idle ();
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}


void reshape(int w, int h) {
    camera.resize (w, h);
}



int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("TP HAI714I");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    //Unit sphere mesh loaded with precomputed normals
    openOFF("data/unit_sphere_n.off", mesh.vertices, mesh.normals, mesh.triangles);

    //Uncomment to see other meshes
    //openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles);

    setUnitSphere( unit_sphere );
    setAdditionnalMesh( additionnal_shape );

    glutMainLoop ();
    return EXIT_SUCCESS;
}

