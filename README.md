# Mesh Segmentation

## Basic definition of mesh segmentation problem:

Let M be a 3D boundary-mesh, and S the set of mesh elements which is either V ,E or F . A segmentation Σ of M is the set of sub-meshes Σ ={M0,...,Mk−1} induced by a partition of S into k disjoint sub-sets

![ProcessOfSegmentation](https://user-images.githubusercontent.com/64921559/136804832-459d4cea-d97f-4742-ad1f-6594a26d4ab3.jpg)


## Algorithms:
  1) Graph-Cut techniques

  2) Region Growing

  3) Hierarchical clustering

  4) Iterative clustering

  5) Spectral analysis


## Graph cut

References: 
[Segmentation and Shape Extraction of 3D Boundary Meshes](https://github.com/timofeysaybel/MeshSegmentation/files/7322933/Segmentation.and.Shape.Extraction.of.3D.Boundary.Meshes.pdf), 

[Graph cut based mesh segmentation using feature points and geodesic distance](https://github.com/timofeysaybel/MeshSegmentation/files/7322936/Graph.cut.based.mesh.segmentation.using.feature.points.and.geodesic.distance.pdf), 

[Hierarchical Mesh Decomposition using Fuzzy Clustering and Cuts](https://github.com/timofeysaybel/MeshSegmentation/files/7322938/Hierarchical.Mesh.Decomposition.using.Fuzzy.Clustering.and.Cuts.pdf), 

[Interactive Graph Cuts for Optimal Boundary & Region Segmentation of Objects in N-D Images](https://github.com/timofeysaybel/MeshSegmentation/files/7322939/Interactive.Graph.Cuts.for.Optimal.Boundary.Region.Segmentation.of.Objects.in.N-D.Images.pdf).


### Algorithm

  1. Assigning distances to all pairs of faces in the mesh.
  2. After computing an initial decomposition, assigning each face a probability of belonging to each patch.
  3. Computing a fuzzy decomposition by refining the probability values using an iterative clustering scheme.
  4. Constructing the exact boundaries between the components, thus transforming the fuzzy decomposition into the final one.

Computational complexity: O(F^2*logF), F - faces.

## Region growing

![CodevRuDME4](https://user-images.githubusercontent.com/64921559/136807392-af47ab47-7039-421c-a66e-98463fc9f3c2.jpg)


Reference:
[Segmentation and Shape Extraction of 3D Boundary Meshes](https://github.com/timofeysaybel/MeshSegmentation/files/7323103/Segmentation.and.Shape.Extraction.of.3D.Boundary.Meshes.pdf),

[A fast and eﬃcient mesh segmentation method based on
improved region growing](https://github.com/timofeysaybel/MeshSegmentation/files/7323108/A.fast.and.e.cient.mesh.segmentation.method.based.on.improved.region.growing.pdf)


### Algorithm

## Project structure
  include(src)/io: constains MeshReader, MeshWritter - read and write .off files
  
  include(src)/model: constains Point, Indices, Face, Vertex, Mesh - classes to work with mesh
  
  include(src)/util: contains util namespace - helper functions (dot, normalize, cross)
  
  include(src)/segmentation: contains segmentation algorithms
  
    include(src)/segmentation/fuzzyClusteringGraphCut: mesh decomposition using fuzzy clustering and cuts. Contains Flow (to solve Max Flow) and GraphMesh
    (extends Mesh)
