# ML_RRT-_APF

We tried to ameliorate the problem of manual setting of Artificial Potential Field constants in the APF woven implementation of the classic RRT* algorithm ( https://github.com/Snehal-Reddy/RRT_Star_With_APF ).

We generated 10,000 images with random obstacles.


![alt_text](https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/img0.jpg)  

We then used BFS and Dijkstra to calculate actual shortest path 


![alt_text](https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/chaljabhai_screenshot_10.12.2018.png) 

Using the above data and using the area of the obstacle and the centre of the obstacle as features , we try to find the APF constants by training.
