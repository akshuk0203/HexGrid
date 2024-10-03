# HexGrid

**Project Overview :**
 
  Use of hexagons to detect and highlight various objects or regions on the grid and to find paths between detected objects, making the system highly functional for mapping and tracking purposes.
  

**Progress :**

  Generating a dynamic grid of hexagons using C for computations and JavaScript for GUI rendering and interaction. The application takes two inputs: the radius of the hexagon (hexRadius) and the resolution level. At each resolution level, every hexagon is split into its seven child hexagons.
    The C program handles all geometric computations, such as calculating the coordinates for each hexagon at various resolution levels. It outputs a structured dataset representing the hexagonal grid, which is then used by JavaScript for visualization.
    The JavaScript front-end provides a graphical user interface (GUI) where users can interact with the hexagon grid. When a user clicks on any hexagon, its unique ID is returned. Additionally, if a user provides a hexagon ID, the corresponding hexagon is highlighted on the grid, enabling easy identification and interaction.

**Ongoing Work :**

  Currently, this hexagonal grid is being further integrated with object detection to enhance its capabilities.
