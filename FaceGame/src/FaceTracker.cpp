//#include <opencv/highgui.h>
//#include <opencv2/opencv.hpp>
//#include <opencv/cv.h>
//#include <opencv/cxcore.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>
//-------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <curses.h>
#include <termios.h>
#include <cstdlib>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <opencv2/highgui/highgui.hpp>
//#include <dlib/image_processing/frontal_face_detector.h>
//#include <dlib/image_processing/render_face_detections.h>
//#include <dlib/image_processing.h>
//#include <dlib/gui_widgets.h>
#include <iostream>
#include <unistd.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv2/face.hpp>
#include <dirent.h>
using namespace std;
using namespace cv;

int64_t t1, t0; //framerate 출력을 위한 변수
CascadeClassifier faceDetector;
cv::Ptr<cv::face::Facemark> facemark;
void display();
void onMouseEvent(int event, int x, int y, int flags, void *i);
void timer_func(int value);
void faceCam();
void overlayImage(const Mat &background, const Mat &foreground, Mat &output, Point2i location);
float cvpt_to_glpt(float x);
float glpt_to_cvpt(float x);
void draw_tri();
std::vector<string> get_files(const char * path);
GLuint MatToTexture(Mat image);
std::vector<int> wSize1(1);
std::vector<int> wSize2(3);

std::vector<std::vector<cv::Point>> pts;
std::vector<std::vector<cv::Point>> facePts;

Mat im;
int use_camera = 1;
std::vector<std::vector<float>> xPts;
std::vector<std::vector<float>> yPts;

//frontal_face_detector detector;
//shape_predictor sp;

float width = 640;
float height = 480;
int life = 3;
cv::VideoCapture camera;
GLuint texture_background;

float radiusRR = 50.0; //정사각형 한변길이

int screenW;
int screenH;

int player_num = -1;

int food_number_global = -1;

struct pos
{
    GLfloat x;
    GLfloat y;
};

struct food 
{
    struct pos pos;
    Mat img;
    int food_number;
};

unsigned char PALETTE[9][3] = {
    {0, 255, 255}, // CYAN
    {0, 255, 0},   // GREEN
    {0, 0, 255},   // BLUE
    {255, 0, 0},   // RED
    {255, 190, 0}, // Orange
    {255, 255, 0}, // YELLOW
    {255, 0, 255}, // PURPLE
    {190, 0, 255}, // Violet
    {0, 0, 0},     // BLACK
};

std::vector<food> ingridents;

string nowLife = "LIFE : ♥♥♡♡";


//struct{
//    int x;
//    int y;
//} pos;

void keyboard(unsigned char key, int x, int y)
{
    //ESC 키가 눌러졌다면 프로그램 종료
    if (key == 27)
    {
        exit(0);
    }
    else if (char(key) == 'd')
    {
       
    }
}
//std::vector<pos> shapes;

void reshape(GLsizei width, GLsizei height)
{
    //cv::Mat temp;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height); //윈도우 크기로 뷰포인트 설정
    glMatrixMode(GL_PROJECTION);                       //이후 연산은 Projection Matrix에 영향을 준다.
    glLoadIdentity();
    gluOrtho2D(-640, 640, -480, 480);
}
void init()
{
    glGenTextures(1, &texture_background);
    
    //화면 지울때 사용할 색 지정
    glClearColor(0.0, 0.0, 0.0, 0.0);
}
int start_game()
{
    
    int argc = 0;
    char **argv = 0;
    glutInit(&argc, argv);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    
    
    wSize1[0] = 7;
    wSize2[0] = 11;
    wSize2[1] = 9;
    wSize2[2] = 7;
    facemark = cv::face::FacemarkLBF::create();
    
    faceDetector.load("../../data/haarcascade_frontalface_alt2.xml");
    facemark->loadModel("../../data/lbfmodel.yaml");
    
    //    detector = get_frontal_face_detector();
    //    deserialize("../../data/shape_predictor_68_face_landmarks.dat")>>sp;
    
    // Mat temp;
    // camera.read(temp);
    // cout<<temp.cols << "  "<< temp.rows;
    //    camera.read(temp);
    screenW = 640;
    screenH = 480;
    glutInitWindowSize(screenW, screenH);
    
    glutCreateWindow("FOOD-FIGHTER");
    init();
    t0 = cvGetTickCount();
    faceCam();
    
    glGenTextures(1, &texture_background);
    
    //화면 지울때 사용할 색 지정
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glutDisplayFunc(display);
    
    //reshape 콜백     함수 등록, reshape함수는 윈도우 처음 생성할 때와 윈도우 크기 변경시 호출된다.
    glutReshapeFunc(reshape);
    //gluOrtho2D(-20, temp.cols, temp.rows, -20);
    //타이머 콜백 함수 등록, 처음에는 바로 호출됨.
    glutTimerFunc(0, timer_func, 0);
    //키보드 콜백 함수 등록, 키보드가 눌러지면 호출된다.
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    
}

int main(int argc, char** argv)
{
    //    player_num = 2;
    
    //    food_number_global=0;

    camera = cv::VideoCapture(0);
    
    camera.set(cv::CAP_PROP_FRAME_WIDTH,1280);
    camera.set(CAP_PROP_FRAME_HEIGHT,720);
    if (!camera.isOpened())
    { //카메라가 제대로 연결되지 않았다면 프로그램 종료
        use_camera = 0;
        std::cout << "please check your camera!" << std::endl;
        return -1;
    }
    //    detector = get_frontal_face_detector();
    //    deserialize("../../data/shape_predictor_68_face_landmarks.dat")>>sp;
    
    camera.read(im);
    
    if(!camera.isOpened())
    {
        exit(-1);
    }
    
    const char *path = "../../image/ingredients/";
    auto files = get_files(path);
    
    // inverse - triangle
    Mat empty;
    food igdts;
    igdts = {100,200, empty, 4};
    ingridents.push_back(igdts);
    
    for (int i = 0; i < files.size(); i++)
    {
        string character = files[i];
        
        if(character.rfind(".",0) == 0)
            continue;
        
        Mat img = cv::imread(path + character);
        cv::resize(img, img, cv::Size(30, 30));
        food igdts;
        
        auto x = std::rand() % im.cols;
        auto y = std::rand() % im.rows;
        if(x<=200)x=300;
        if(y<=200)y=300;
        if(x>=im.cols-300)x=im.cols-300;
        if(y<=im.rows-300)y=im.rows-300;
        auto tp_x = cvpt_to_glpt((float)x);
        auto tp_y = cvpt_to_glpt((float)y);
        
        if(character.rfind("b",0) == 0)
        {
            igdts = {{tp_x,tp_y},img, 0};
        }
        else if(character.rfind("p",0) == 0)
        {
            igdts = {{tp_x,tp_y},img, 1};
        }
        else if(character.rfind("h",0) == 0)
        {
            igdts = {{tp_x,tp_y},img, 2};
        }
        ingridents.push_back(igdts);
    }
    Mat image = cv::imread("../../image/intro_c.PNG", cv::IMREAD_COLOR);
    namedWindow("FOOD-FIGHTER", WINDOW_GUI_NORMAL);
    resizeWindow("FOOD-FIGHTER", 640, 480);
    setMouseCallback("FOOD-FIGHTER", onMouseEvent);
    
    imshow("FOOD-FIGHTER", image);
    waitKey(0);
    
    
}


void draw_glvertex(int max, int y)
{
    glBegin(GL_POLYGON);
    glVertex2i(max / 2, y);             //1
    glVertex2i((max / 2) - 10, y + 10); //2
    glVertex2i((max / 2) - 30, y + 10); //3
    glVertex2i((max / 2) - 35, y + 5);  //4
    glVertex2i((max / 2) - 35, y - 10); //5
    glVertex2i(max / 2, y - 50);        //6
    glVertex2i((max / 2) + 35, y - 10); //7
    glVertex2i((max / 2) + 35, y + 5);  //8
    glVertex2i((max / 2) + 30, y + 10); //9
    glVertex2i((max / 2) + 10, y + 10); //10
    glEnd();
    
}

void draw_life()
{
    glColor3f(1.0, 0.0, 0.0); //도형색을 빨간색으로 지정
    int max = 1200;
    int y = -430;
    if (life == 1)
    {
        draw_glvertex(max, y);
    }
    else if (life == 2)
    {
        draw_glvertex(max, y);
        max = 1000;
        draw_glvertex(max, y);
        
    }
    else if (life == 3)
    {
        draw_glvertex(max, y);
        max = 1000;
        draw_glvertex(max, y);
        max = 800;
        draw_glvertex(max, y);
        
    }
}

float cvpt_to_glpt(float x)
{
    x = (float)(x-(float)640/2.0)*2;
    
}


float glpt_to_cvpt(float x)
{
    auto out =x/2 + 640/2;
    return out;
}

void CVtoGL() // OpenCV의 좌표를 OpenGL좌표에 맞춰 변환
{
    
    int w = 640;
    int h = 480;
    xPts.clear();
    yPts.clear();
    for (int j =0; j < pts.size(); j++) {
        
        
        std::vector<float> xpts;
        std::vector<float> ypts;
        for (int i = 0; i < 6; i++)
        {
            xpts.push_back((float)((float)pts[j][i].x - (float)w / 2.0) * 2.0);
            ypts.push_back(-(float)((float)pts[j][i].y - (float)h / 2.0) * 2.0);
        }
        xPts.push_back(xpts);
        yPts.push_back(ypts);
    }
    
    
    //cout << endl;
}

GLuint MatToTexture(Mat image) //OpenCV의 Mat이미지를 gluint 텍스처로 맵핑
{
    if (image.empty())
        return -1;
    
    //OpenGL 텍스처 생성
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    //텍스처 ID를 바인딩 -  사용할 텍스처 차원을 지정해준다.
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows,
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.ptr());
    
    return textureID;
}

void endGame(int success = 0)
{
    life = 0;
    glutLeaveMainLoop();
    
    sleep(1);
    //    glutLeaveMainLoop(); //glut의 반복문을 벗아남
    namedWindow("GAME END", WINDOW_GUI_NORMAL);
    resizeWindow("GAME END", 640, 480);
    
    Mat image;
    if(success == 0)
    {
        image = cv::imread("../../image/OVER_c.PNG", cv::IMREAD_COLOR);
    }
    else{
        image = cv::imread("../../image/success.png", cv::IMREAD_COLOR);
    }
    
    imshow("GAME END", image);
    
    waitKey(0);
}


void desLife() // 생명 감소시 호출되는 함수
{
    
    cv::Scalar cc(0, 0, 255); //빨간색 지정
    Mat copy;
    double alpha = 0.5;
    
    life--; //life 감소
    if (life == 0) //life가 0이되는 경우
    {
        
        im.copyTo(copy); //추가
        //    fillConvexPoly(copy, facePts, 27, cc);
        addWeighted(copy, alpha, im, 1 - alpha, 0, im); //추가
        
        
        endGame(); //endGame 함수 호출
    }
}

void draw_background() //크기에 맞춰 배경 그림
{
    int x = screenW;
    int y = screenH;
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-x, -y, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(x, -y, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(x, y, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-x, y, 0.0);
    glEnd();
}
int inTriColorIndex = 7;
void inTriColor()
{
    GLfloat Red = PALETTE[inTriColorIndex][0] / 255.0f;
    GLfloat Green = PALETTE[inTriColorIndex][1] / 255.0f;
    GLfloat Blue = PALETTE[inTriColorIndex][2] / 255.0f;
    glColor3f(Red, Green, Blue);
}

void draw_tri()
{
    for(auto item:ingridents)
    {
        auto food_number = item.food_number;
        if(food_number == 4)
        {
            auto x = item.pos.x;
            auto y = item.pos.y;
            inTriColor();
            //glColor3f(255, 255, 255);
            glBegin(GL_TRIANGLES);
            glVertex2f(x - 25, y + 2 * 25);
            glVertex2f(x + 25, y + 2 * 25);
            glVertex2f(x, y);
            glEnd();
        }
    }
}

Mat draw_food(Mat im) // show ingredients on the top
{
    Mat overlay;
    im.copyTo(overlay);
    int x_loc = im.cols - 100;
    int y_loc = 25;
    for(auto item:ingridents)
    {
        if(food_number_global == item.food_number){
            y_loc+=item.img.rows+item.img.rows/4;
            auto x = x_loc;
            auto y = y_loc;
            item.img.copyTo(overlay(cv::Rect(x,y,item.img.cols, item.img.rows)));
        }
    }
    
    
    for (int i =0; i < ingridents.size(); i++) {
        auto item = ingridents[i];
        if(item.food_number == 4)continue;
        
        auto x = item.pos.x;
        auto y = item.pos.y;
        auto food_number = item.food_number;
        
        x = glpt_to_cvpt(x);
        y = glpt_to_cvpt(y);
        
        if(x<=150)
            x = 150;
        if(y<item.img.rows*2)
            y = item.img.rows*2;
        if(x>= overlay.cols - item.img.cols*2)
            x=overlay.cols - item.img.cols*2;
        if(y>=overlay.rows-item.img.rows*3)
            y = overlay.rows-item.img.rows*3;
        
        item.img.copyTo(overlay(cv::Rect(x,y,item.img.cols, item.img.rows)));   
    }
    return overlay;
}

void random_move()
{
    for (size_t i = 0; i <ingridents.size(); i++) {
        int plus_mnius_x = std::rand()%2;
        int plus_mnius_y = std::rand()%2;
        int posx = std::rand() % 50;
        int posy = std::rand() % 50;
        if(plus_mnius_x == 0) posx = posx*-1;
        if(plus_mnius_y == 0) posy = posy*-1;
        auto item = ingridents.at(i);
        
        food ingrident;
        
        if(item.food_number == 4)
        {
            posx += item.pos.x;
            posy += item.pos.y;
            if(posx  <-640)posx  = -640;
            
            if( posy  <-640)posy  = -640;
            
            if(posx >740)posx =740;
            
            if( posy >740) posy =740;
            ingrident.pos.x = posx;
            ingrident.pos.y = posy;
        }
        else
        {
            posx += item.pos.x;
            posy += item.pos.y;
            //            if(posx  <item.img.rows*3)posx  = item.img.rows*3;
            
            //            if( posy  <item.img.rows*2)posy  = item.img.rows*2;
            
            //            if(posx >im.cols - item.img.cols*2)posx =im.cols-item.img.cols*2;
            
            //            if( posy >im.rows - item.img.rows*3) posy =im.rows - item.img.rows*3;
            
            ingrident.pos.x = posx;
            ingrident.pos.y = posy;
        } 
        ingridents.at(i).pos= ingrident.pos;
    }
}

void display() //계속해서 호출되는 디스플레이 함수
{
    pthread_t p[4];
    int err;
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // background color
    
    texture_background = MatToTexture(im); //gl 이미지로 맵핑
    if (texture_background < 0)
        return;
    
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); //큐브나 좌표축 그릴 때 사용한 색의 영향을 안받을려면 필요
    
    glBindTexture(GL_TEXTURE_2D, texture_background);
    draw_background();
    glPushMatrix();
    draw_tri();
    
    std::vector<int> remove_idx;
    
    for(int i =0; i<pts.size(); i++)
    {
        for (int j = 0; j < ingridents.size();j++)
        {
            auto pos = ingridents[j].pos;
            
            if(ingridents[j].food_number == 4)
            {
                if(xPts[i][0]  < pos.x && xPts[i][2] > pos.x && (pos.y-25) < yPts[i][2] && (pos.y+25)>yPts[i][3] )
                {
                    ingridents[j].pos.x = -200;
                    ingridents[j].pos.y = -200;
                    desLife();
                }
            }
            else
            {
                if(xPts[i][0]  < pos.x +ingridents[j].img.cols&& xPts[i][2] > pos.x-ingridents[j].img.cols && (pos.y-ingridents[j].img.cols) < yPts[i][2] && (pos.y+ingridents[j].img.cols)>yPts[i][3] )
                {
                    remove_idx.push_back(j);
                    ingridents[j].pos.x = -9999;
                }
            }
        }
    }
    for(auto elemen:remove_idx)
    {
        int remove_idx = -99;
        for(int i = 0; i<ingridents.size(); i++)
        {
            if(ingridents[i].pos.x == -9999)
            {
                remove_idx = i;
                break;
            }
        }
        ingridents.erase(ingridents.begin()+ remove_idx);
    }
    int exit = 0;
    for(auto element:ingridents)
    {
        if(element.food_number == food_number_global)
        {
            exit = -1;
        }
        
    }
    if(exit == 0)
    {
        endGame(1);
    }
    
    random_move();
    
    draw_life();
    
    glPopMatrix();
    glutSwapBuffers();
}

bool started = false;
bool menu = false;

std::vector<string> get_files(const char * path)
{
    DIR *dir;
    struct dirent *ent;
    std::vector<string> files;
    if ((dir = opendir (path)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            //        printf ("%s\n", ent->d_name);
            files.push_back(ent->d_name);
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        exit(-1);
    }
    return files;
}

void onMouseEvent(int event, int x, int y, int flags, void *i = 0)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        if(x>150 && x<333 && y>330 && y<405 && started == true)
        {
            player_num = 1;
        }
        
        if(x>440 && x<610 && y>340 && y<410 && started == true)
        {
            player_num = 2;
        }
        
        if((player_num == 1 || player_num==2) && menu == false)
        {
            started = false;
            menu = true;
            Mat image = cv::imread("../../image/menu_c.PNG", cv::IMREAD_COLOR);
            imshow("FOOD-FIGHTER", image);
            waitKey(1);
        }
        
        if(menu)
        {
            int x1 = 692;
            int x2 = 860;
            if(x>x1 && x<x2&& y>256 && y<315)
            {
                food_number_global = 0;
            }
            else if(x>x1 && x<x2&& y>417 && y<470)
            {
                
                food_number_global = 1;
            }
            if(x>x1 && x<x2&& y>584 && y<640)
            {
                
                food_number_global = 2;
            }
            
        }
        
        if(started == false && menu == true && food_number_global !=-1)
        {
            cvDestroyWindow("FOOD-FIGHTER");
            start_game();
        }
        
        if (x > 144 && x < 307 && y > 240 && y < 303 && started == false)
        {
            started = true;
            Mat image = cv::imread("../../image/players_c.PNG", cv::IMREAD_COLOR);
            imshow("FOOD-FIGHTER", image);
            waitKey(1);
        }
    }
}

void timer_func(int value)
{
    faceCam();
    im = draw_food(im);
    cvtColor(im, im, CV_BGR2RGB);
    glutPostRedisplay();        //윈도우를 다시 그리도록 요청
    glutTimerFunc(1, timer_func, 0); //다음 타이머 이벤트는 1밀리세컨트 후  호출됨.
}

//void find_face(cv::Mat &image, )

void faceCam()
{
    
    string text, dynamic_score;
    char sss[256];
    char ooo[256];
    double fps = 0;
    Mat frame, gray;
    bool show = true;
    camera.read(frame);
    frame.copyTo(im);
    if(use_camera == 1)
    {
        cv::flip(frame,frame,1);
        cv::flip(im, im, 1);
    }
    
    //  cv_image<bgr_pixel> cimg(im);
    
    
    //  std::vector<dlib::rectangle> dets = detector(cimg);
    
    //  std::vector<full_object_detection> shapes;
    //  for (unsigned long j = 0; j < player_num; ++j)
    //  {
    
    //      auto shape = sp(cimg, dets[j]);
    
    //      std::vector<cv::Point> facepts;
    //      std::vector<cv::Point> lippts;
    //      for (unsigned long i = 0; i< shape.num_parts() ;i++) {
    
    
    
    //          if(i<=27)
    //          {
    //              auto pt = shape.part(i);
    //              auto pt_cv = cv::Point(int(pt.x()), int(pt.y()));
    //              facepts.push_back(pt_cv);
    //          }
    //          if(i>=60 && i<=65)
    //          {
    //              auto pt = shape.part(i);
    //              auto pt_cv = cv::Point(int(pt.x()), int(pt.y()));
    //              lippts.push_back(pt_cv);
    //          }
    
    
    
    //      }
    ////      for(int i = 0; i <68; i++)
    ////      {
    ////          if(i<=27)
    ////          {
    ////              auto pt_cv = cv::Point(int(0), int(0));
    ////              facepts.push_back(pt_cv);
    ////          }
    ////          if(i>=60 && i<=65)
    ////          {
    ////              auto pt_cv = cv::Point(int(0), int(0));
    ////              lippts.push_back(pt_cv);
    ////          }
    ////      }
    
    
    //      facePts.push_back(facepts);
    //      pts.push_back(lippts);
    
    //  }
    
    std::vector<Rect> faces;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    faceDetector.detectMultiScale(gray, faces);
    std::vector< std::vector<Point2f> > landmarks;
    bool success = facemark->fit(frame,faces,landmarks);
    
    if(success)
    {
        
        facePts.clear();
        pts.clear();
        for (size_t i =0; i<faces.size(); i++)
        {
            std::vector<cv::Point> facepts;
            std::vector<cv::Point> lippts;
            for (size_t j = 0; j<landmarks[i].size();j++)
            {
                if(j<=27)
                {
                    auto pt_cv = landmarks[i][j];
                    //                  auto pt_cv = cv::Point(int(pt.x()), int(pt.y()));
                    facepts.push_back(pt_cv);
                }
                if(j>=60 && j<=65)
                {
                    auto pt_cv = landmarks[i][j];
                    //                  auto pt_cv = cv::Point(int(pt.x()), int(pt.y()));
                    lippts.push_back(pt_cv);
                }
            }
            facePts.push_back(facepts);
            pts.push_back(lippts);
        }
    }
    CVtoGL();
    for(auto pt:pts)
    {
        cv::fillConvexPoly(im, pt.data(), 6, CV_RGB(255, 0, 0));
    }
    
    t1 = cvGetTickCount();
    fps = 60 - ((double(t1 - t0) / cvGetTickFrequency()) / 1e+6);
    if(fps<=0)
    {
        endGame();
    }
    
    if (show)
    {
        sprintf(sss, "time limit : %d", (int)round(fps));
        //sprintf(ooo, "LIFE : %d", life);
        //nowLife = ooo;
        //putText(im, nowLife, cv::Point(500, 470), CV_FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(0, 0, 0), 2);
        putText(im, sss, cv::Point(50, 50), CV_FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(255, 255, 255),2);
    }
}