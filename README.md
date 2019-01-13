# Deferred-Shading-Scene
The project is about a deferred shading to show a scene constructing in a json file use OpenGL 3.3,<br>
you can make your own scene with the same format json.<br>
Now the result scene is only rendered by some random point light<br>
![image](https://github.com/zhoumingyang/Deferred-Shading-Scene/blob/master/showjson/image/scene-oneroom.png)<br>
![image](https://github.com/zhoumingyang/Deferred-Shading-Scene/blob/master/showjson/image/scene-threeroom1.png)<br>
![image](https://github.com/zhoumingyang/Deferred-Shading-Scene/blob/master/showjson/image/scene-threeroom2.png)<br>
<br>
## Feature in Project
1. use JsonCpp to parse a scene json<br>
2. use deferred shading to draw the whole scene<br>
3. use assimp to import obj file<br>
4. releaize some basic light source including direction light,point light and spot light<br>
5. releaize basic camera operation<br>
## json format<br>
the json is divided by scenes, each scene has obj files and meshes wich is design by user<br>
![image](https://github.com/zhoumingyang/Deferred-Shading-Scene/blob/master/showjson/image/scenefile.png)<br>
