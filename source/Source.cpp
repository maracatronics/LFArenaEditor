#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define NUMPATTERNS 8
#define OFFSETX 25
#define OFFSETY 25

using namespace cv;

int pieces[20][20];

int selectedPiece = 1;

Mat patterns[NUMPATTERNS];

Mat screen;
Mat menu;

int mousex = -1;
int mousey = -1;

void reset() {
	memset(pieces, 0, sizeof(int)*400);
}

void save() {
	Mat saveMat;
	saveMat.create(600, 600, CV_8UC3);
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			Rect WhereRec(j * 30, i * 30, 30, 30);
			patterns[pieces[i][j]].copyTo(saveMat(WhereRec));
		}
	}
	imwrite("LFArena.png", saveMat);
	printf("File successfully saved!\n");
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		//cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		if (x > 650 + 21 && x < 650 + 21 + 59) {
			if (y > 21 && y < 21 + 7 * 60 + 59) {
				selectedPiece = (y - 21) / 60;
			}
			else if(y > 513 && y < 513 + 60) {
				// fire reset action
				//printf("reset\n");
				reset();
			}
			else if (y > 513 + 60 && y < 513 + 120) {
				// fire save action
				//printf("save\n");
				save();
			}
		}
		else if (x > OFFSETX && x < OFFSETX + 600 && y > OFFSETY && y < OFFSETY + 600) {
			int coordX = (x - OFFSETX) / 30;
			int coordY = (y - OFFSETY) / 30;
			pieces[coordY][coordX] = selectedPiece;
		}
 
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MBUTTONDOWN)
	{
		//cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		//cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
	}
}

void loadPatterns() {
	char filename[200];
	for (int i = 0; i < NUMPATTERNS; i++) {
		sprintf(filename, "pieces/%02d.png", i);
		patterns[i] = imread(filename, 1);
	}
}


void drawYellowGrid() {

	for (int i = 0; i <= 20; i++) {
		line(screen, Point(OFFSETX+30 * i, OFFSETY + 0), Point(OFFSETX + 30 * i, OFFSETY + 600), Scalar(0, 255, 255), 1);
		line(screen, Point(OFFSETX+0, OFFSETY + 30 * i), Point(OFFSETX + 600, OFFSETY + 30 * i), Scalar(0, 255, 255), 1);
	}

}

void drawPatterns() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			Rect WhereRec(OFFSETX + j*30, OFFSETY + i*30, 30, 30);
			patterns[pieces[i][j]].copyTo(screen(WhereRec));
		}
	}
}

void drawMenu() {
	Rect WhereRec(650, 0, 100, 650);
	menu.copyTo(screen(WhereRec));

	rectangle(screen, Point(650 + 21, 21+selectedPiece*59), Point(650 + 21 + 59, 21 + selectedPiece * 59 + 59), Scalar(0, 255, 255));
}

void main() {

	namedWindow("Line Follower Arena Editor", 1);

	//set the callback function for any mouse event
	setMouseCallback("Line Follower Arena Editor", CallBackFunc, NULL);

	menu = imread("menu.png", 1);

	loadPatterns();

	screen.create(650, 750, CV_8UC3);

	while (true) {
		screen = 0;
		drawPatterns();
		drawYellowGrid();
		drawMenu();

		imshow("Line Follower Arena Editor", screen);
		waitKey(10);

	}


}