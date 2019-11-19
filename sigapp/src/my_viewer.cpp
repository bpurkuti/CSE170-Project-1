
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

double x= 0.0;
double y = 65;
double z = 0.0;
float degrees = (float)GS_PI / 180;

//These variables keep track of where the joints are
float leftarmcntr = 0;
float rightarmcntr = 0;
float leftlegcntr = 0;
float rightlegcntr = 0;
float headcntr = 0;


MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	_nbut=0;
	_animating=false;
	build_ui ();
	build_scene ();

}

void MyViewer::build_ui ()
{
	UiPanel *p;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "Animate", EvAnimate ) );
	p->add ( new UiButton ( "Exit", EvExit ) ); p->top()->separate();
}

void MyViewer::add_model ( SnShape* s, GsVec p )
{
	SnManipulator* manip = new SnManipulator;
	GsMat m;


	m.translation ( p );
	manip->initial_mat ( m );

	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	manip->child(g);
	manip->visible(false); // call this to turn off mouse interaction
	rootg()->add(manip);
}

void MyViewer::build_scene ()
{
	rootg()->remove_all();
	//SnPrimitive* p;

	////Floor
	//p = new SnPrimitive(GsPrimitive::Box,35,0.5,35);
	//p->prim().material.diffuse=GsColor::darkred;
	//GsModel* d = p->model();
	//d->translate(GsVec(0, -1300, 0));
	//d->scale(35);
	//add_model(p, GsVec(x,y,z));

	//Right Leg
	//load the obj to the model
	//rotate the matrices and transform to change the alignment
	//Translate to the proper place
	//scale it to change the size
	SnModel* leg = new SnModel;
	leg->model()->load_obj("../objs/leg.obj");
	GsModel* o = leg->model();
	GsMat lmatz;
	lmatz.rotx(-100 * degrees);
	o->transform(lmatz, false);
	o->translate(GsVec(-15, -50, 0));
	o->scale(13);
	add_model(leg, GsVec(x, y, z));

	//Left Leg
	//Same step as left leg
	//I am mirroring the obj in x axis
	//Using Identity matrix mirr and multiplying it to input matrix lrmatz, then transforming it
	//The model appears black because I believe its turning it inside out, do not know how to fix it yet
	SnModel* legr = new SnModel;
	legr->model()->load_obj("../objs/leg.obj");
	GsModel* lr = legr->model();
	GsMat lrmatz;
	GsMat mirr(-1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	lrmatz.rotx(-100 * degrees);
	GsMat mirrmat = lrmatz * mirr;
	lr->transform(mirrmat, false);
	lr->translate(GsVec(13, -50, 0));
	lr->scale(13);
	add_model(legr, GsVec(x, y, z));
	

	//Torso
	SnModel* torso = new SnModel;
	torso->model()->load_obj("../objs/torso.obj");
	GsModel* t = torso->model();
	GsMat tmat;
	tmat.rotx(-100 * degrees);
	t->transform(tmat, false);
	t->translate(GsVec(0, 50, -20));
	t->scale(18);
	add_model(torso, GsVec(x, y, z));

	//Head
	SnModel* head = new SnModel;
	head->model()->load_obj("../objs/TheRock2.obj");
	GsModel* h = head->model();
	h->translate(GsVec(-15, 915, -210));
	h->scale(2);
	add_model(head, GsVec(x, y, z));
	
	//Right Arm
	SnModel* rarm = new SnModel;
	rarm->model()->load_obj("../objs/arm.obj");
	GsModel* rm = rarm->model();
	GsMat ramat;
	ramat.rotx(-100 * degrees);
	rm->transform(ramat, false);
	rm->translate(GsVec(-40, 60, -20));
	rm->scale(15);
	add_model(rarm, GsVec(x, y, z));

	////Left Arm
	SnModel* larm = new SnModel;
	larm->model()->load_obj("../objs/arm.obj");
	GsModel* lm = larm->model();
	GsMat lamat;
	//I am going to use the same GsMat mirr
	lamat.rotx(-100 * degrees);
	GsMat mirr2mat = lamat * mirr;
	lm->transform(mirr2mat, false);
	lm->translate(GsVec(35, 60, -20));
	lm->scale(15);
	add_model(larm, GsVec(x, y, z));
}

void MyViewer::moveleftarm(float x) 
{
	SnManipulator* larm = rootg()->get<SnManipulator>(5);
	GsMat armMat = larm->mat();
	GsMat tr;
	GsMat rot;
	leftarmcntr += x;
	rot.rotx(leftarmcntr);
	tr.translation(GsVec(0, 0, 0));
	
	armMat.rotz(leftarmcntr* degrees);
	armMat.mult(tr, armMat);
	larm->initial_mat(armMat);
	render();
	ws_check();


}

void MyViewer::moverightarm(float x)
{
	SnManipulator* larm = rootg()->get<SnManipulator>(4);
	GsMat armMat = larm->mat();
	GsMat tr;
	GsMat rot;
	rightarmcntr+= x;
	rot.rotz(rightarmcntr);
	tr.translation(GsVec(0, 0, 0));

	armMat.rotx(rightarmcntr*degrees);
	armMat.mult(tr, armMat);
	larm->initial_mat(armMat);
	render();
	ws_check();
}

void MyViewer::moveleftleg(float x) 
{
	SnManipulator* larm = rootg()->get<SnManipulator>(1);
	GsMat armMat = larm->mat();
	GsMat tr;
	GsMat rot;
	leftlegcntr += x;
	rot.rotz(leftlegcntr);
	tr.translation(GsVec(0, 0, 0));

	armMat.rotx(leftlegcntr);
	armMat.mult(tr, armMat);
	larm->initial_mat(armMat);
	render();
	ws_check();
}

void MyViewer::moverightleg(float x)
{
	SnManipulator* larm = rootg()->get<SnManipulator>(0);
	GsMat armMat = larm->mat();
	GsMat tr;
	GsMat rot;
	rightlegcntr += x;
	rot.rotx(rightlegcntr);
	tr.translation(GsVec(0, 0, 0));

	armMat.rotz(rightlegcntr);
	armMat.mult(tr, armMat);
	larm->initial_mat(armMat);
	render();
	ws_check();
}

void MyViewer::movehead(float x)
{
	SnManipulator* larm = rootg()->get<SnManipulator>(3);
	GsMat armMat = larm->mat();
	GsMat tr;
	GsMat rot;
	headcntr += x;
	rot.rotz(headcntr);
	tr.translation(GsVec(0, 0, 0));

	armMat.rotz(headcntr);
	armMat.mult(tr, armMat);
	larm->initial_mat(armMat);
	render();
	ws_check();
}

void MyViewer::moveall(float x) 
{

}
// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation ()
{
	if ( _animating ) return; // avoid recursive calls
	_animating = true;
	
	int ind = gs_random ( 0, rootg()->size()-1 ); // pick one child
	SnManipulator* manip = rootg()->get<SnManipulator>(ind); // access one of the manipulators
	GsMat m = manip->mat();

	double frdt = 1.0/30.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double t=0, lt=0, t0=gs_time();
	do // run for a while:
	{	while ( t-lt<frdt ) { ws_check(); t=gs_time()-t0; } // wait until it is time for next frame
		double yinc = (t-lt)*v;
		if ( t>2 ) yinc=-yinc; // after 2 secs: go down
		lt = t;
		m.e24 += (float)yinc;
		if ( m.e24<0 ) m.e24=0; // make sure it does not go below 0
		manip->initial_mat ( m );
		render(); // notify it needs redraw
		ws_check(); // redraw now
	}	while ( m.e24>0 );
	_animating = false;
}


int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		default: gsout<<"Key pressed: "<<e.key<<gsnl;
	
	case 'q':
	{
		moverightarm(-0.001f);
		return 1;
	}
	case 'a': 
	{
		moverightarm(0.001f);
		return 1;
	}

	
	case 'w':
	{
		moveleftarm(-0.001f);
		return 1;
	}

	case 's':
	{
		moveleftarm(0.001f);
		return 1;
	}

	case 'e':
	{
		moverightleg(-0.01f);
		return 1;
	}
	case 'd':
	{
		moverightleg(0.01f);
		return 1;
	}
	case 'r':
	{
		moveleftleg(-0.01f);
		return 1;
	}
	case 'f':
	{
		moveleftleg(0.01f);
		return 1;
	}
	case 'z':
	{
		movehead(-0.001f);
		return 1;
	}
	case 'x':
	{
		movehead(0.001f);
		return 1;
	}

	case GsEvent::KeySpace:
	{
		double lt, t0 = gs_time();
		do
		{
			lt = gs_time() - t0;

			//camera().eye.x += 1.0f;
			camera().eye.y += 2.5f;
			camera().eye.z -= 3.0f;
			//camera().center.y += 1.0f;
			//camera().center.x += 1.0f;

			camera().up.z += 0.001f;
			render();
			ws_check();
			message().setf("local time=%f", lt);
		} while (lt < 1.5f);
	}
	}

	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	
		case EvAnimate: run_animation(); return 1;
		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
