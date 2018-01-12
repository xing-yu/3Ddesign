Readme file


The program was complied and tested on macOS High Sierra


How to execute file:
	Using terminal to go the folder and run ./TDDE


Layout:
	Left panel -- diplay 3D object
	Right panel -- draw curve

Mourse:
	Left panel:
		normal mode -- click and drag the mouse to pitch and yaw
		roll mode -- click and drag to roll cameral
		(default is normal mode, use keyborad 'r' to toggle modes)

	Right panel:
		add point -- add new points
		select point --  select and move a point
		(default is add point mode, use right click to pull the menu to change mode)

Keyboard:
	arrow keys: slide camera
	z, x: zoom camera
	t: toggle between wireframe and filled mode for 3D object (default is filled)
	1: toggle on and off light source 1 (default on, only works in filled mode)
	2: toggle on and off light source 2 (default on, move with camera, only works in filled mode)
	e: toggle on and off texture mode (default off, only works in filled mode)
	+: increase the degree of the Bezier curve, max 50
	-: decrease the degree of the Bezier curve, min 1
	d: delete selected control point, only works in edit mode
	c: toggle on and off control polyline for the Bezier curve
	space: reset camera
	q: quit program

Texture file:
	1. must be ppm format
	2. should be named as "tp.ppm"
	3. should be in the same directory
