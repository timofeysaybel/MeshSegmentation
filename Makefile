buildGraphCut:
	mkdir build && cd build && cmake ../code/src/segmentation/fuzzyClusteringGraphCut/ && make

buildRegionGrowing:
	mkdir build && cd build && cmake ../code/src/segmentation/regionGrowing/ && make

build2off:
	mkdir build && cd build && cmake ../code/src/seg2off/ && make

buildCmp:
	mkdir build && cd build && cmake ../code/src/cmpSeg/ && make

clean:
	rm -r build mesh/res/*
