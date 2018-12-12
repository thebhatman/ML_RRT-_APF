# ML_RRT*_APF

We tried to ameliorate the problem of manual setting of Artificial Potential Field constants in the APF woven implementation of the classic RRT* algorithm ( https://github.com/Snehal-Reddy/RRT_Star_With_APF ).

![alt_text](https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/maxresdefault.jpg)  

We generated 1,00,000 images with random obstacles.They can be accessed at this link-
https://drive.google.com/open?id=19T3MWMWzlgY7_ybz7Ial-A5Ixky1jM2q


![alt_text](https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/img0.jpg)  

We then used BFS and Dijkstra to calculate actual shortest path (since it was unweighted).


![alt_text](https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/chaljabhai_screenshot_10.12.2018.png) 

Using the above data and using the area of the obstacle and the centre of the obstacle as features , we try to find the APF constants by training.

The results turned out pretty well and the tree converged well towards the target . 

<p align="center">
  <img src="https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/Tree_with_ML.png"/>
</p>


![alt_text](https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/Tree_with_ML.png)






