--RUNNING THIS SCRIPT RESETS THE CURRENT MAXFILE WITHOUT PROMPT.
(
resetmaxfile #noPrompt

function testIsSoftIsHard = 
(
	--Test smooth sphere.
	local s = convertTo (sphere smooth:true) Editable_Poly;
	local numE = polyOp.getNumEdges s;
	--Test all edges together.
	assert (EdgeSmooth.IsSoft s #{1..e}) message:"Expected all edges in smooth sphere to be soft";
	assert (not (EdgeSmooth.IsHard s #{1..e})) message:"Expected all edges in smooth sphere to not be hard";
	--Test individual edges
	for e = 1 to numE do
	(
		assert (EdgeSmooth.IsSoft s #{e}) message:("Expected edge #" + (e as string) + " in smooth sphere to be soft");
		assert (not (EdgeSmooth.IsHard s #{e})) message:("Expected edge #" + (e as string) + " in smooth sphere to not be hard");
	)
	
	--Test unsmoothed sphere.
	s = convertTo (sphere smooth:false) Editable_Poly;
	numE = polyOp.getNumEdges s;
	--Test all edges together.
	assert (not (EdgeSmooth.IsSoft s #{1..e})) message:"Expected all edges in non-smooth sphere to not be soft";
	assert (EdgeSmooth.IsHard s #{1..e}) message:"Expected all edges in non-smooth sphere to be hard";
	--Test individual edges
	for e = 1 to numE do
	(
		assert (not (EdgeSmooth.IsSoft s #{e})) message:("Expected edge #" + (e as string) + " in non-smooth sphere to not be soft");
		assert (EdgeSmooth.IsHard s #{e}) message:("Expected edge #" + (e as string) + " in non-smooth sphere to be hard");
	)
	
	--Test plane (only border edges)
	local p = convertTo (Plane lengthSegs:1 widthSegs:1) Editable_Poly;
	numE = polyOp.getNumEdges p;
	assert (not (EdgeSmooth.IsSoft s #{1..e})) message:"Expected all borders to not be soft";
	assert (EdgeSmooth.IsHard s #{1..e}) message:"Expected all borders to be hard";
	--Test individual edges
	for e = 1 to numE do
	(
		assert (not (EdgeSmooth.IsSoft s #{e})) message:("Expected border #" + (e as string) + "  to not be soft");	
		assert (EdgeSmooth.IsHard s #{e}) message:("Expected border #" + (e as string) + "  to be hard");	
	)
)



function testApply = 
(
   local s = convertTo (sphere smooth:true) Editable_Poly;
	local numE = polyOp.getNumEdges s;
	local edge = random 1 numE;
	EdgeSmooth.Apply false node:s edges:#{edge}
	assert (EdgeSmooth.IsHard s #{edge}) message:("Apply(false) on #" + edge as string);
	
	s = convertTo (sphere smooth:false) Editable_Poly;
	numE = polyOp.getNumEdges s;
	edge = random 1 numE;
	EdgeSmooth.Apply true node:s edges:#{edge}
	assert (EdgeSmooth.IsSoft s #{edge}) message:("Apply(true) on #" + edge as string)
)


testIsSoftIsHard();
testApply();

resetmaxfile #noPrompt
)