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
{<br>
    "scenes": [<br>
        {<br>
            "sceneName": "none",<br>
            "sceneId": "1",<br>
            "objs": [<br>
                {<br>
                    "id": "1",<br>
                    "pos": [x, y, z],<br>
                    "rot": [x, y, z, w],<br>
                    "scale": [1,1,1],<br>
                    "modelName": "name",<br>
                    "modelPath": "./obj/nameid_model.obj",<br>
                    "imgPath": "./res/nameid_model.png",<br>
                    "color": [r, g, b]/<br>
                }<br>
				{<br>
					...<br>
				}/<br>
				...<br>
            ],/<br>
            "meshes": [<br>
                {<br>
                    "pos": [x, y, z],<br>
                    "rot": [x, y, z, w],<br>
                    "scale": [1,1,1],<br>
                    "id": "1",<br>
                    "meshName": "name",<br>
                    "normals": [x, y, z, x, y, z,...]<br>
                    "uvs": [u,v,...],<br>
                    "points": [x,y,z...],<br>
                    "indices": [1,2,3...],<br>
                    "imgPath": "./res/nameId.jpg",<br>
                    "normalImgPath": "",<br>
                    "color": [r,g,b,a],<br>
                    "doubleSide": true/<br>
                },<br>
				{<br>
					...<br>
				}<br>
				...<br>
            ]<br>
        }<br>
    ]<br>
}<br>
