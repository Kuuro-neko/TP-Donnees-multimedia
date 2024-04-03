// --------------------------------------------------------------------------
// gMini,
// a minimal Glut/OpenGL app to extend                              
//
// Copyright(C) 2007-2009                
// Tamy Boubekeur
//                                                                            
// All rights reserved.                                                       
//                                                                            
// This program is free software; you can redistribute it and/or modify       
// it under the terms of the GNU General Public License as published by       
// the Free Software Foundation; either version 2 of the License, or          
// (at your option) any later version.                                        
//                                                                            
// This program is distributed in the hope that it will be useful,            
// but WITHOUT ANY WARRANTY; without even the implied warranty of             
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              
// GNU General Public License (http://www.gnu.org/licenses/gpl.txt)           
// for more details.                                                          
//                                                                          
// --------------------------------------------------------------------------

uniform float ambientRef;
uniform float diffuseRef;
uniform float specularRef;
uniform float shininess;

uniform float levels;
uniform int shading;

varying vec4 p;
varying vec3 n;

void main (void) {
    vec3 P = vec3 (gl_ModelViewMatrix * p); //Position du point à éclairer
    vec3 N = normalize (gl_NormalMatrix * n); //Normal en ce point
    vec3 V = normalize (-P); //Vecteur de vue
    
    vec4 Isa = gl_LightModel.ambient;
    vec4 Ka = gl_FrontMaterial.ambient;
    vec4 Ia = Isa * Ka;

    vec4 I = ambientRef * Ia ;
    
    ////////////////////////////////////////////////
    //Eclairage de Phong à calculer en utilisant
    ///////////////////////////////////////////////
    // gl_LightSource[i].position.xyz Position de la lumière i
    // gl_LightSource[i].diffuse Couleur diffuse de la lumière i
    // gl_LightSource[i].specular Couleur speculaire de la lumière i
    // gl_FrontMaterial.diffuse Matériaux diffus de l'objet
    // gl_FrontMaterial.specular Matériaux speculaire de l'objet

    gl_FragColor = vec4 (I.xyz, 1);
    if (shading == 0) {
        for (int i = 0; i < 3; i++) {
            float dotLN = dot(normalize(gl_LightSource[i].position.xyz - P), N);
            vec4 Id = gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse * dotLN;
            vec4 D = diffuseRef * Id;

            vec3 R = float(2) * dot(N, normalize(gl_LightSource[i].position.xyz - P)) * N - normalize(gl_LightSource[i].position.xyz - P);
            vec4 Is = gl_LightSource[i].specular * gl_FrontMaterial.specular * pow(dot(R, V), shininess);
            vec4 S = specularRef * Is;

            gl_FragColor += vec4(D.xyz, 1) + vec4(S.xyz, 1);
        }
    } else {
        if (dot(N, V) < 0.25) {
            gl_FragColor = vec4(0, 0, 0, 1);
        } else {
            // vec3 L = gl_ModelViewMatrix + gl_LightSource[1].position.xyz;
            vec3 L = gl_LightSource[0].position.xyz;
            float dotLN = dot(normalize(L - P), N);
            dotLN = floor((dotLN + 1.0) * (levels/2.0)) / (levels/2.0) - 1.0;
            vec4 Id = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * dotLN;
            vec4 D = diffuseRef * Id;

            vec3 R = float(2) * dot(N, normalize(L - P)) * N - normalize(L - P);
            vec4 Is = gl_LightSource[0].specular * gl_FrontMaterial.specular * pow(dot(R, V), shininess);
            vec4 S = specularRef * Is;
            gl_FragColor += vec4(D.xyz, 1) + vec4(S.xyz, 1);
        }
    }
}

