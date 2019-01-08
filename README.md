# ML_RRT*_APF

[![Built with ❤](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com/#)

[![Chat at Slack](https://img.shields.io/badge/chat-on%20slack-black.svg?style=for-the-badge)](https://join.slack.com/t/pathplanningl-dyr9182/shared_invite/enQtNTEwMjk3MzgzNjIwLWRlNjA4Nzk4MDk4MzcxNjNmYmRkODlmM2Y0MDc1ODEyZDM1MDA5Y2IyODM1ZjgzNDBmMDU5YmJiN2UwMjg4NjQ)

We tried to ameliorate the problem of manual setting of Artificial Potential Field constants in the APF woven implementation of the classic RRT* algorithm ( https://github.com/Snehal-Reddy/RRT_Star_With_APF ).

![alt_text](https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/maxresdefault.jpg)  

We generated 1,00,000 images with random obstacles.They can be accessed at this link-
https://drive.google.com/open?id=1_RFMAEEHSNluhC_CLTgPwLQ8VW1Pq-zF


![alt_text](https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/img0.jpg)  

We then used BFS and Dijkstra to calculate actual shortest path (since it was unweighted).


![alt_text](https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/chaljabhai_screenshot_10.12.2018.png) 

Using the above data and using the area of the obstacle and the centre of the obstacle as features , we try to find the APF constants by training.

The results turned out pretty well and the tree converged well towards the target . 

<p align="center">
  <img src="https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/Tree_with_ML.png" width = "200" height = "200">
</p>


<p align="center">
  <img src="https://github.com/Snehal-Reddy/ML_RRT-_APF/blob/master/images/Tree_with_ML1.png" width = "200" height = "200">
</p>
###Aproach 2###
We use OMPL's inbuilt RRT-X to get the ideal path. 
We then build a neural network which learns by comparing its path with the RRT-X one without actually reverse engineering ideal potential constants from the ideal path.
Research Paper is currently under development.




