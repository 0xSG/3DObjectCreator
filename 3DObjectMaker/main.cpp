#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "math.h"
#include <vector>

// DEV :SURYA GANGARAJ K //// S G K //// 0xpulsar - GitHub
/* THIS PROJECT IS OPEN SOURCED UNDER APACHE LICENSE V2.0 */
/*typedef*/
typedef struct Point {
    GLfloat x, y, z;
    Point(GLfloat px, GLfloat py, GLfloat pz) { x = px;y = py;z = pz;   }
    bool operator== (const Point &c1) { return (x == c1.x && y == c1.y);    }
    
} point;
typedef std::vector<point> Line;



static std::vector<point> Cline;
static std::vector<Line> lines;
static GLfloat size = -0.3;
static GLfloat vision [3]={0.0,45.0,1.0};
static GLdouble theta[3] = {0.0, 0.0, 0.0};
static GLfloat colors [] [3]={{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,1.0}};
static GLint color=3;
static float height=500,width=400,hMid=250,wMid=200;
static GLint axis = 2;




point converter(point val){
    point t(0.0,0.0,0.0);
    t.x=(val.x-wMid)/200;
    t.y=(val.y-hMid)/200;
    t.z=(val.z)/2;
    return t;
}
bool checkIsFirst(Point p1, std::vector<point> line) {
    if (line.size() == 0) return 0;
    else {
        point t = line.front();
        if(t==p1){
            return true;
        } else
            return false;
    }
}

point pointSimplifier(point p1, std::vector<point> line) {
    point pres = p1;
    long double mag = 100;
    for (point p2 : line) {
        mag = sqrt(pow(p1.x - p2.x, 2.0) + pow(p1.y - p2.y, 2.0) + pow(p1.z - p2.z, 2.0));
        if (mag < 7) {
            pres = p2;
            break;
        }
    }
    return pres;
}

void drawDot(point p1){
    glPushMatrix();
    glTranslatef(p1.x  , p1.y, p1.z);
    glutSolidSphere(1.0, 100, 100);
    glPopMatrix();
}

void projectAllDots(){
    for(point p: Cline){
        drawDot(p);
    }
}

void makeLine(point p1, point p2) {
    GLfloat pf1[3]={p1.x, p1.y, p1.z};
    GLfloat pf2[3]={p2.x, p2.y, p2.z};
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(colors[color][0],colors[color][1],colors[color][2]);
    
    glLineWidth(3);
    glVertex3fv(pf1);
    glVertex3fv(pf2);
    glEnd();
    glPopMatrix();
}

void fillSides( std::vector<point> line){
    for(point p:line){
        point p1(p.x,p.y,0.0);
        point p2(p.x,p.y,size);
        makeLine(p1, p2);
    }
}

void drawLineLoop( std::vector<point> line){
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    glLineWidth(10);
    for (point p2 : line) {
        const GLfloat v[3]={p2.x, p2.y, p2.z};
        glColor3f(colors[color][0],colors[color][1],colors[color][2]);
        //printf("\n%f %f %f \n",(float)v[0],(float) v[1],(float) v[2]);
        glVertex2fv(v);
    }
    glPopMatrix();
    glEnd();

    
}

void drawLines() {
    
    point pre(0,0,0);
    int size = Cline.size();
    if (size > 1) {
        pre = Cline.front();
        for (point p : Cline) {
            makeLine(pre, p);
            pre.x = p.x;
            pre.y = p.y;
            pre.z = p.z;
        }
        
    }else{
        pre=Point(0,0,0);
    }
}

Line convertToIdentityForm(Line l){
    Line t;
    for(point p:l){
        p=converter(p);
        t.push_back(p);
    }
    return t;
}

void displayW1(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawLines();
    projectAllDots();
    glFlush();
}

void displayW2(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //DEV :SURYA GANGARAJ K //// S G K //// 0xpulsar - GitHub
    //printf("\nvision> %d %d %d \n",(int)vision[0],(int) vision[1],(int) vision[2]);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    
    glLoadIdentity();
    if(lines.size()>0){
        
        
        for(Line l: lines){
            l=convertToIdentityForm(l);
            glRotatef(theta[1], 1.0, 0.0, 0.0);
            glRotatef(theta[2], 0.0, 1.0, 0.0);
            glRotatef(theta[3], 0.0, 0.0, 1.0);
            glPushMatrix();
            
            drawLineLoop(l);
            fillSides(l);
            glTranslatef(0.0, 0.0, size);
            drawLineLoop(l);
            
            glPopMatrix();
            glEnd();
        }
        
    }
    glFlush();
}



void mouseW1(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        point CP(x, (500 - y), 1.0);
        CP = pointSimplifier(CP, Cline);
       
        if (checkIsFirst(CP, Cline)) {
            //loop formed..
            printf("loop formed..");
            
            lines.push_back(Cline);
            
            Cline.clear();
        } else {
            Cline.push_back(CP);
        }
    }
    glutPostRedisplay();
}

void mouseW2(int button, int state, int x, int y) {
    if(axis==2){
        axis=0;
        
    }else        axis++;
    glutPostRedisplay();
}

void mykeyboardW1(unsigned char key, int x, int y) {
    int k = (int) key - '0';
    printf("key_code =%d  \n", k);
    if (!Cline.empty() && k == 74)
        Cline.pop_back();
    
    glutPostRedisplay();
}

void mykeyboardW2(unsigned char key, int x, int y) {
    printf("%c",key);
    switch (key) {
        case 'w':
            color=3;
            break;
        case 'r':
            //red
            color=0;
            break;
        case 'g':
            color=1;
            break;
        case 'b':
            color=2;
            break;
        case 'i':
            size-=0.01;
            break;
        case 'd':
            size+=0.01;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}


void spingCube(){
    theta[axis] +=1.0;
    if(theta[axis]>360) theta[axis]-=360;
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    
    //DEV :SURYA GANGARAJ K //// S G K //// 0xpulsar - GitHub
    /*Window 2 */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100,150);
    glutInitWindowSize(600, 600);
    glutCreateWindow("3D Viewer");
    glutKeyboardFunc(mykeyboardW2);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutIdleFunc(spingCube);
    gluOrtho2D(0, 400, 0, 500);
    glutDisplayFunc(displayW2);
    glutMouseFunc(mouseW2);
    glEnable(GL_DEPTH_TEST);
    
    /*Window 1 */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowPosition(800,150);
    glutInitWindowSize(width, height);
    glutCreateWindow("Draw Here");
    glutKeyboardFunc(mykeyboardW1);
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(0, 400, 0, 500);
    glutDisplayFunc(displayW1);
    glutMouseFunc(mouseW1);
    
    glutMainLoop();            /* start processing events... */
    
    return 0;
}


