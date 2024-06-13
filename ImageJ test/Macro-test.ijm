//setTool("line");
/*
makeLine(1, 358, 1000, 357);
Length = 1;
Length = getNumber("Type the known length of Line :", Length);
run("Set Scale...", "distance=1000 known="+Length+" unit=um");
*/
write("The number of slices is :"+nSlices);
for(i=0; i<nSlices; i++){
	run("Translate...", "x=0 y=100 interpolation=None slice");
	run("Next Slice [>]");
	write(i);
}