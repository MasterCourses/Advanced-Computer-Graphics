# Ray_Tracer_BVH_Mesh_Speedup_for_TXT_File
## Goal 
To increase the experimental speed of the previous assignment.

## Approach 
- Bounding Volume Hierarchy (BVH) with Triangle Mesh. <br>
  The idea is to construct a box to surround each object and detect whether the emitted ray touches the box, <br>
  and then detect whether intersects the objects inside the box.
  
## Experimental Results
- ### Output Image
<p align="center"><img src="https://github.com/NUZEROVI/Ray_Tracer_BVH_Mesh_Speedup_for_TXT_File/blob/Speedup_CG_HW/Results.png"></p>

- ### Run time comparison
<p align="center"><img src="https://github.com/NUZEROVI/Ray_Tracer_BVH_Mesh_Speedup_for_TXT_File/blob/Speedup_CG_HW/Comparisons.png"></p>
