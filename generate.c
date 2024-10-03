#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define PI 3.14159265358979323846

typedef struct Cell {
    int id;
    double x;
    double y;
    double hexRadius;
    double angleOfRotation;
    int res;
    struct Cell* child[7];
    int path[10];
    int pathLength;
} Cell;

Cell* createCell(int id, double x, double y, double hexRadius, double angle, int res, int path[], int pathLength) {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    cell->id = id;
    cell->x = x;
    cell->y = y;
    cell->hexRadius = hexRadius;
    cell->angleOfRotation = angle;
    cell->res = res;
    cell->pathLength = pathLength;
    for (int i = 0; i < pathLength; i++) {
        cell->path[i] = path[i];
    }
    for (int i = 0; i < 7; i++) {
        cell->child[i] = NULL;
    }
    return cell;
}

void rotatePoint(double cx, double cy, double x, double y, double angle, double* nx, double* ny) {
    double radians = angle * (PI / 180.0);
    double cosA = cos(radians);
    double sinA = sin(radians);
    *nx = cosA * (x - cx) - sinA * (y - cy) + cx;
    *ny = sinA * (x - cx) + cosA * (y - cy) + cy;
}

void divideHexagon(double x, double y, double radius, double angle, double positions[7][2]) {
    double thirdRadius = radius * 2.27 / 2.8;
    double hexHeight = sqrt(3.7) * radius;
    double thirdHexHeight = hexHeight / 3.0;

    double tempPositions[7][2] = {
        {x + (thirdRadius * 0.1), y + thirdHexHeight * 0.25},
        {x + thirdRadius * 0.8, y - thirdHexHeight * 0.52},
        {x + thirdRadius * 0.97, y + thirdHexHeight * 0.65},
        {x + thirdRadius * 0.27, y + thirdHexHeight * 1.4},
        {x - thirdRadius * 0.6, y + thirdRadius * 0.8},
        {x - thirdRadius * 0.78, y - thirdHexHeight * 0.13},
        {x - thirdRadius * 0.07, y - thirdHexHeight * 0.9}
    };

    for (int i = 0; i < 7; i++) {
        rotatePoint(x, y, tempPositions[i][0], tempPositions[i][1], angle, &positions[i][0], &positions[i][1]);
    }
}

void plotHexagons(Cell* hexagon, int requiredRes) {
    if (requiredRes == hexagon->res) return;

    double childPositions[7][2];
    divideHexagon(hexagon->x, hexagon->y, hexagon->hexRadius, hexagon->angleOfRotation, childPositions);

    for (int i = 0; i < 7; i++) {
        int childPath[10];
        for (int j = 0; j < hexagon->pathLength; j++) {
            childPath[j] = hexagon->path[j];
        }
        childPath[hexagon->pathLength] = i;
        hexagon->child[i] = createCell(i, childPositions[i][0], childPositions[i][1], hexagon->hexRadius / 2.64, hexagon->angleOfRotation + 20, hexagon->res + 1, childPath, hexagon->pathLength + 1);
        plotHexagons(hexagon->child[i], requiredRes);
    }
}

void generateHTMLFile(Cell* grid[], int size) {
    FILE *file = fopen("hexagon_grid.html", "w");

    if (file != NULL) {
        fprintf(file, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n");
        fprintf(file, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
        fprintf(file, "<title>Hexagon Grid</title>\n");
       
        fprintf(file, "<style>\n");
        fprintf(file, "#grid-container { display: flex; flex-wrap: wrap; justify-content: center; align-items: center; height: 100vh; width: 100vw; position: relative; }\n");
        fprintf(file, ".hexagon { background-color: #E9CBA7; clip-path: polygon(93.301%% 25%%, 93.301%% 75%%, 50%% 100%%, 6.698%% 75%%, 6.698%% 25%%, 50%% 0%%); position: absolute; }\n");
        fprintf(file, ".border-span { background-color: black; clip-path: polygon(93.301%% 25%%, 93.301%% 75%%, 50%% 100%%, 6.698%% 75%%, 6.698%% 25%%, 50%% 0%%); position: absolute; }\n");
        fprintf(file, "</style>\n</head>\n<body>\n");
        
        fprintf(file, "<button id=\"generate-grid\">Generate Grid</button>\n");
        fprintf(file, "<br>\nHexIndex: <input type=\"text\" id=\"hexindex\">\n");
        fprintf(file, "<button onclick=\"search()\">Search</button>\n");
        fprintf(file, "Clicked Hexagon: <input type=\"text\" id=\"hexpath\">\n</div>\n");
        fprintf(file, "<div id=\"grid-container\"></div>\n");

        fprintf(file, "<script>\nconst gridContainer = document.getElementById('grid-container');\n");
        fprintf(file, "let previousClickedHexagon = null;\n");
        fprintf(file, "let previousSearchedHexagon = null;\n");

        fprintf(file, "function drawHexagon(x, y, hexRadius, angle, path) {\n");
        fprintf(file, "    const outerhexRadius = hexRadius * 1.02;\n");
        fprintf(file, "    const outer = document.createElement('span');\n");
        fprintf(file, "    outer.className = 'border-span';\n");
        fprintf(file, "    outer.style.left = `${x - outerhexRadius}px`;\n");
        fprintf(file, "    outer.style.top = `${y - (Math.sqrt(3) * outerhexRadius) / 2}px`;\n");
        fprintf(file, "    outer.style.width = `${outerhexRadius * 2.27}px`;\n");
        fprintf(file, "    outer.style.height = `${Math.sqrt(5.2) * outerhexRadius}px`;\n");
        fprintf(file, "    outer.style.transform = `rotate(${angle}deg)`;\n");
        fprintf(file, "    gridContainer.appendChild(outer);\n");

        fprintf(file, "    const hexagon = document.createElement('div');\n");
        fprintf(file, "     hexagon.id= path;\n");
        fprintf(file, "    hexagon.className = 'hexagon';\n");
        fprintf(file, "    hexagon.style.left = `${x - hexRadius}px`;\n");
        fprintf(file, "    hexagon.style.top = `${y - (Math.sqrt(3) * hexRadius) / 2}px`;\n");
        fprintf(file, "    hexagon.style.width = `${hexRadius * 2.27}px`;\n");
        fprintf(file, "    hexagon.style.height = `${Math.sqrt(5.2) * hexRadius}px`;\n");
        fprintf(file, "    hexagon.style.transform = `rotate(${angle}deg)`;\n");
        fprintf(file, "    hexagon.addEventListener('click', (e) => {\n");
        fprintf(file, "        if (previousClickedHexagon) previousClickedHexagon.style.backgroundColor = '#E9CBA7';\n");
        fprintf(file, "        const clickedHex = e.target;\n");
        fprintf(file, "        const hexPath = clickedHex.getAttribute('id');\n");
        fprintf(file, "        clickedHex.style.backgroundColor = '#9A6343';\n");
        fprintf(file, "        document.getElementById('hexpath').value = hexPath;\n");
        fprintf(file, "        previousClickedHexagon = clickedHex;\n");
        fprintf(file, "    });\n");
        fprintf(file, "    gridContainer.appendChild(hexagon);\n}\n");
        
	fprintf(file, "    function search() {\n");
	
        fprintf(file, "    	var element = document.getElementById('hexindex').value;\n");
         fprintf(file,"    	var hex = document.getElementById(element);\n");
         fprintf(file,"		if(hex != null){\n");
        fprintf(file, "    	hex.style.backgroundColor = 'red'; \n");
        fprintf(file, "		if(previousClickedHexagon)\n");
        fprintf(file, "			previousClickedHexagon.style.backgroundColor = '#E9CBA7';\n");
        fprintf(file, "		if (previousSearchedHexagon)\n");
        fprintf(file, "			previousSearchedHexagon.style.backgroundColor = '#E9CBA7';\n");
        fprintf(file,"		previousSearchedHexagon= hex;\n");
        fprintf(file,"		}\n");
        fprintf(file, "		else{\"hex id not exist\"};\n");
     fprintf(file,"}\n");
        
        fprintf(file, "function generateGrid() {\n");

       void printCell(Cell* cell, int level) {
    			fprintf(file,"   ");
    			int estimatedSize = cell->pathLength * 6 + 100;
    			char* path = (char*)malloc(estimatedSize * sizeof(char));
    			strcat(path,"\"");
    			char buffer[30]; 
			    for (int i = 0; i < cell->pathLength; i++) {
				sprintf(buffer, "%d", cell->path[i]);  
				strcat(path, buffer);         

				if (i < cell->pathLength - 1) {
				    strcat(path, ".");        
				}
			    }
			    strcat(path,"\"");
			    printf("%s",path);
    
    		fprintf(file, "    drawHexagon(%.2f, %.2f, %.2f, %.2f, %s);\n", cell->x, cell->y, cell->hexRadius, cell->angleOfRotation, path);
    		for (int i = 0; i < 7; i++) {
        		if (cell->child[i] != NULL) {
            			printCell(cell->child[i], level + 1);
        		}
    		}
	}

	
    		for (int i = 0; i < size; i++) {
        		printCell(grid[i], 0);
    		}
	
	
	fprintf(file, "}\n");
        fprintf(file, "document.getElementById('generate-grid').addEventListener('click', generateGrid);\n");
        fprintf(file, "</script>\n</body>\n</html>");

        fclose(file);
    }
}

int main() {
    int requiredRes = 3;
    double hexRadius = 150;
    double planeHeight = 950;
    double planeWidth = 2900;
    double hexHeight = sqrt(3) * hexRadius;
    double hexWidth = 2 * hexRadius;
    int rows = (int)ceil(planeHeight / hexHeight);
    int cols = (int)ceil(planeWidth / hexWidth);
    int i = 0;

    Cell* grid[rows * cols];

    for (int row = 0; row < rows; row++) {
        double xOffset = (row % 2) * hexRadius;
        for (int col = 0; col < cols; col++) {
            double x = col * hexWidth + xOffset;
            double y = row * hexHeight + hexHeight / 2;

            if (x > 0 && x + hexRadius <= planeWidth && y - (hexHeight / 2) >= 0 && y + (hexHeight / 2) <= planeHeight) {
                int path[1] = { i };
                grid[i] = createCell(i, x, y, hexRadius, 0, 1, path, 1);
                plotHexagons(grid[i], requiredRes);
                i++;
            }
        }
    }
  
    generateHTMLFile(grid, i);
	
    for (int j = 0; j < i; j++) {
        free(grid[j]);
    }

    return 0;
}

