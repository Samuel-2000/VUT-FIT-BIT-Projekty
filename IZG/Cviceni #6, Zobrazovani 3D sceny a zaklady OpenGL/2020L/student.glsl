#ifdef VERTEX_SHADER
//#define MAKE_STUDENT_RELEASE
uniform mat4 view;
uniform mat4 proj;
#line 5

out vec2 vCoord;
//out int index;


void main(){

    if(gl_VertexID>=12) {
        gl_Position = vec4(0,0,0,1);
        return;
    }  

   // int index = gl_VertexID;

    mat4 mvp = proj*view;
      // postava
    if(gl_VertexID==0+0){vCoord = vec2(0,0);gl_Position = mvp*vec4(-2,0,-2,1);}
    if(gl_VertexID==1+0){vCoord = vec2(1,0);gl_Position = mvp*vec4(+2,0,-2,1);}
    if(gl_VertexID==2+0){vCoord = vec2(0,1);gl_Position = mvp*vec4(-2,0,+2,1);}
    if(gl_VertexID==0+3){vCoord = vec2(0,1);gl_Position = mvp*vec4(-2,0,+2,1);}
    if(gl_VertexID==1+3){vCoord = vec2(1,0);gl_Position = mvp*vec4(+2,0,-2,1);}
    if(gl_VertexID==2+3){vCoord = vec2(1,1);gl_Position = mvp*vec4(+2,0,+2,1);}

      // stena
    if(gl_VertexID==0+0){vCoord = vec2(0,0);gl_Position = mvp*vec4(-1,-1,0,1);}
    if(gl_VertexID==1+0){vCoord = vec2(1,0);gl_Position = mvp*vec4(+1,-1,0,1);}
    if(gl_VertexID==2+0){vCoord = vec2(0,1);gl_Position = mvp*vec4(-1,+1,0,1);}
    if(gl_VertexID==0+3){vCoord = vec2(0,1);gl_Position = mvp*vec4(-1,+1,0,1);}
    if(gl_VertexID==1+3){vCoord = vec2(1,0);gl_Position = mvp*vec4(+1,-1,0,1);}
    if(gl_VertexID==2+3){vCoord = vec2(1,1);gl_Position = mvp*vec4(+1,+1,0,1);}
}
#endif



#ifdef FRAGMENT_SHADER
out highp vec4 fColor;

in vec2 vCoord;
//in int index;

void main(){

  //  if(ID>=0 && ID<6) {
        vec2 uv = vCoord*4.f;
        float f = 1.0;
        for(int k = 0; k < 3; k++) {
            if(uv.x > f && uv.x < f + 1.0)uv.x-=f;
            if(uv.y > f && uv.y < f + 1.0)uv.y-=f;
            f++;
        }

        float d = distance(uv,vec2(0.5));
        float c=float(d<0.5);

        vec3 col = mix(vec3(0,0.7,0), vec3(0,1,0),c);

        fColor = vec4(col,1);
  //  }

/*
    if(ID>=6 && ID<12) {
        vec2 uv = vCoord;


        float c=float(uv.x>.1&&uv.x<.9&&uv.y>.1&&uv.y<.9);

        vec4 col = mix(vec4(0,0,0,1), vec4(1,1,0,.5),c);

        fColor = vec4(col);
    }

    //fColor = vec4(col,0.5); // pruhlednost
*/
}
#endif