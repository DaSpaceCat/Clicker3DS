#include "torus.h"
const vertex v1 = {1.250000 , 0.000000 , 0.000000};
const vertex v2 = {1.125000 , 0.216506 , 0.000000};
const vertex v3 = {0.875000 , 0.216506 , 0.000000};
const vertex v4 = {0.750000 , 0.000000 , 0.000000};
const vertex v5 = {0.875000 , -0.216506, 0.000000};
const vertex v6 = {1.125000 , -0.216506, 0.000000};
const vertex v7 = {1.082532 , 0.000000 , -0.625000};
const vertex v8 = {0.974279 , 0.216506 , -0.562500};
const vertex v9 = {0.757772 , 0.216506 , -0.437500};
const vertex v10 = {0.649519 , 0.000000 , -0.375000};
const vertex v11 = {0.757772 , -0.216506, -0.437500};
const vertex v12 = {0.974279 , -0.216506, -0.562500};
const vertex v13 = {0.625000 , 0.000000 , -1.082532};
const vertex v14 = {0.562500 , 0.216506 , -0.974279};
const vertex v15 = {0.437500 , 0.216506 , -0.757772};
const vertex v16 = {0.375000 , 0.000000 , -0.649519};
const vertex v17 = {0.437500 , -0.216506, -0.757772};
const vertex v18 = {0.562500 , -0.216506, -0.974279};
const vertex v19 = {0.000000 , 0.000000 , -1.250000};
const vertex v20 = {0.000000 , 0.216506 , -1.125000};
const vertex v21 = {0.000000 , 0.216506 , -0.875000};
const vertex v22 = {0.000000 , 0.000000 , -0.750000};
const vertex v23 = {0.000000 , -0.216506, -0.875000};
const vertex v24 = {0.000000 , -0.216506, -1.125000};
const vertex v25 = {-0.625000, 0.000000 , -1.082532};
const vertex v26 = {-0.562500, 0.216506 , -0.974279};
const vertex v27 = {-0.437500, 0.216506 , -0.757772};
const vertex v28 = {-0.375000, 0.000000 , -0.649519};
const vertex v29 = {-0.437500, -0.216506, -0.757772};
const vertex v30 = {-0.562500, -0.216506, -0.974279};
const vertex v31 = {-1.082532, 0.000000 , -0.625000};
const vertex v32 = {-0.974278, 0.216506 , -0.562500};
const vertex v33 = {-0.757772, 0.216506 , -0.437500};
const vertex v34 = {-0.649519, 0.000000 , -0.375000};
const vertex v35 = {-0.757772, -0.216506, -0.437500};
const vertex v36 = {-0.974278, -0.216506, -0.562500};
const vertex v37 = {-1.250000, 0.000000 , -0.000000};
const vertex v38 = {-1.125000, 0.216506 , -0.000000};
const vertex v39 = {-0.875000, 0.216506 , -0.000000};
const vertex v40 = {-0.750000, 0.000000 , -0.000000};
const vertex v41 = {-0.875000, -0.216506, -0.000000};
const vertex v42 = {-1.125000, -0.216506, -0.000000};
const vertex v43 = {-1.082532, 0.000000 , 0.625000};
const vertex v44 = {-0.974279, 0.216506 , 0.562500};
const vertex v45 = {-0.757772, 0.216506 , 0.437500};
const vertex v46 = {-0.649519, 0.000000 , 0.375000};
const vertex v47 = {-0.757772, -0.216506, 0.437500};
const vertex v48 = {-0.974279, -0.216506, 0.562500};
const vertex v49 = {-0.625000, 0.000000 , 1.082532};
const vertex v50 = {-0.562500, 0.216506 , 0.974279};
const vertex v51 = {-0.437500, 0.216506 , 0.757772};
const vertex v52 = {-0.375000, 0.000000 , 0.649519};
const vertex v53 = {-0.437500, -0.216506, 0.757772};
const vertex v54 = {-0.562500, -0.216506, 0.974279};
const vertex v55 = {0.000000 , 0.000000 , 1.250000};
const vertex v56 = {0.000000 , 0.216506 , 1.125000};
const vertex v57 = {0.000000 , 0.216506 , 0.875000};
const vertex v58 = {0.000000 , 0.000000 , 0.750000};
const vertex v59 = {0.000000 , -0.216506, 0.875000};
const vertex v60 = {0.000000 , -0.216506, 1.125000};
const vertex v61 = {0.625000 , 0.000000 , 1.082532};
const vertex v62 = {0.562500 , 0.216506 , 0.974279};
const vertex v63 = {0.437500 , 0.216506 , 0.757772};
const vertex v64 = {0.375000 , 0.000000 , 0.649519};
const vertex v65 = {0.437500 , -0.216506, 0.757772};
const vertex v66 = {0.562500 , -0.216506, 0.974279};
const vertex v67 = {1.082532 , 0.000000 , 0.625000};
const vertex v68 = {0.974279 , 0.216506 , 0.562500};
const vertex v69 = {0.757772 , 0.216506 , 0.437500};
const vertex v70 = {0.649519 , 0.000000 , 0.375000};
const vertex v71 = {0.757772 , -0.216506, 0.437500};
const vertex v72 = {0.974279 , -0.216506, 0.562500};

const vertex n1 = {0.8436 , 0.4871 , -0.2260};
const vertex n2 = {-0.0000, 1.0000 , -0.0000};
const vertex n3 = {-0.8436, 0.4871 , 0.2260};
const vertex n4 = {-0.8436, -0.4871, 0.2260};
const vertex n5 = {-0.0000, -1.0000, -0.0000};
const vertex n6 = {0.8436 , -0.4871, -0.2260};
const vertex n7 = {0.6176 , 0.4871 , -0.6176};
const vertex n8 = {-0.6176, 0.4871 , 0.6176};
const vertex n9 = {-0.6176, -0.4871, 0.6176};
const vertex n10 = {0.6176 , -0.4871, -0.6176};
const vertex n11 = {0.2260 , 0.4871 , -0.8436};
const vertex n12 = {-0.2260, 0.4871 , 0.8436};
const vertex n13 = {-0.2260, -0.4871, 0.8436};
const vertex n14 = {0.2260 , -0.4871, -0.8436};
const vertex n15 = {-0.2260, 0.4871 , -0.8436};
const vertex n16 = {0.2260 , 0.4871 , 0.8436};
const vertex n17 = {0.2260 , -0.4871, 0.8436};
const vertex n18 = {-0.2260, -0.4871, -0.8436};
const vertex n19 = {-0.6176, 0.4871 , -0.6176};
const vertex n20 = {0.6176 , 0.4871 , 0.6176};
const vertex n21 = {0.6176 , -0.4871, 0.6176};
const vertex n22 = {-0.6176, -0.4871, -0.6176};
const vertex n23 = {-0.8436, 0.4871 , -0.2260};
const vertex n24 = {0.8436 , 0.4871 , 0.2260};
const vertex n25 = {0.8436 , -0.4871, 0.2260};
const vertex n26 = {-0.8436, -0.4871, -0.2260};

const vertexObj vertex_list[432] {
	{ v1, n1 }, { v8, n1 }, { v2, n1 },
	{ v8, n2 }, { v3, n2 }, { v2, n2 },
	{ v3, n3 }, { v10, n3 }, { v4, n3 },
	{ v10, n4 }, { v5, n4 }, { v4, n4 },
	{ v5, n5 }, { v12, n5 }, { v6, n5 },
	{ v6, n6 }, { v7, n6 }, { v1, n6 },
	{ v13, n7 }, { v8, n7 }, { v7, n7 },
	{ v8, n2 }, { v15, n2 }, { v9, n2 },
	{ v9, n8 }, { v16, n8 }, { v10, n8 },
	{ v16, n9 }, { v11, n9 }, { v10, n9 },
	{ v17, n5 }, { v12, n5 }, { v11, n5 },
	{ v18, n10 }, { v7, n10 }, { v12, n10 },
	{ v19, n11 }, { v14, n11 }, { v13, n11 },
	{ v20, n2 }, { v15, n2 }, { v14, n2 },
	{ v21, n12 }, { v16, n12 }, { v15, n12 },
	{ v22, n13 }, { v17, n13 }, { v16, n13 },
	{ v23, n5 }, { v18, n5 }, { v17, n5 },
	{ v24, n14 }, { v13, n14 }, { v18, n14 },
	{ v19, n15 }, { v26, n15 }, { v20, n15 },
	{ v26, n2 }, { v21, n2 }, { v20, n2 },
	{ v27, n16 }, { v22, n16 }, { v21, n16 },
	{ v28, n17 }, { v23, n17 }, { v22, n17 },
	{ v29, n5 }, { v24, n5 }, { v23, n5 },
	{ v30, n18 }, { v19, n18 }, { v24, n18 },
	{ v31, n19 }, { v26, n19 }, { v25, n19 },
	{ v32, n2 }, { v27, n2 }, { v26, n2 },
	{ v33, n20 }, { v28, n20 }, { v27, n20 },
	{ v34, n21 }, { v29, n21 }, { v28, n21 },
	{ v35, n5 }, { v30, n5 }, { v29, n5 },
	{ v36, n22 }, { v25, n22 }, { v30, n22 },
	{ v31, n23 }, { v38, n23 }, { v32, n23 },
	{ v32, n2 }, { v39, n2 }, { v33, n2 },
	{ v39, n24 }, { v34, n24 }, { v33, n24 },
	{ v40, n25 }, { v35, n25 }, { v34, n25 },
	{ v41, n5 }, { v36, n5 }, { v35, n5 },
	{ v42, n26 }, { v31, n26 }, { v36, n26 },
	{ v43, n3 }, { v38, n3 }, { v37, n3 },
	{ v44, n2 }, { v39, n2 }, { v38, n2 },
	{ v45, n1 }, { v40, n1 }, { v39, n1 },
	{ v46, n6 }, { v41, n6 }, { v40, n6 },
	{ v47, n5 }, { v42, n5 }, { v41, n5 },
	{ v48, n4 }, { v37, n4 }, { v42, n4 },
	{ v49, n8 }, { v44, n8 }, { v43, n8 },
	{ v44, n2 }, { v51, n2 }, { v45, n2 },
	{ v51, n7 }, { v46, n7 }, { v45, n7 },
	{ v52, n10 }, { v47, n10 }, { v46, n10 },
	{ v53, n5 }, { v48, n5 }, { v47, n5 },
	{ v48, n9 }, { v49, n9 }, { v43, n9 },
	{ v55, n12 }, { v50, n12 }, { v49, n12 },
	{ v56, n2 }, { v51, n2 }, { v50, n2 },
	{ v57, n11 }, { v52, n11 }, { v51, n11 },
	{ v58, n14 }, { v53, n14 }, { v52, n14 },
	{ v59, n5 }, { v54, n5 }, { v53, n5 },
	{ v60, n13 }, { v49, n13 }, { v54, n13 },
	{ v61, n16 }, { v56, n16 }, { v55, n16 },
	{ v62, n2 }, { v57, n2 }, { v56, n2 },
	{ v63, n15 }, { v58, n15 }, { v57, n15 },
	{ v64, n18 }, { v59, n18 }, { v58, n18 },
	{ v65, n5 }, { v60, n5 }, { v59, n5 },
	{ v66, n17 }, { v55, n17 }, { v60, n17 },
	{ v67, n20 }, { v62, n20 }, { v61, n20 },
	{ v68, n2 }, { v63, n2 }, { v62, n2 },
	{ v69, n19 }, { v64, n19 }, { v63, n19 },
	{ v64, n22 }, { v71, n22 }, { v65, n22 },
	{ v65, n5 }, { v72, n5 }, { v66, n5 },
	{ v72, n21 }, { v61, n21 }, { v66, n21 },
	{ v67, n24 }, { v2, n24 }, { v68, n24 },
	{ v68, n2 }, { v3, n2 }, { v69, n2 },
	{ v3, n23 }, { v70, n23 }, { v69, n23 },
	{ v4, n26 }, { v71, n26 }, { v70, n26 },
	{ v5, n5 }, { v72, n5 }, { v71, n5 },
	{ v72, n25 }, { v1, n25 }, { v67, n25 },
	{ v1, n1 }, { v7, n1 }, { v8, n1 },
	{ v8, n2 }, { v9, n2 }, { v3, n2 },
	{ v3, n3 }, { v9, n3 }, { v10, n3 },
	{ v10, n4 }, { v11, n4 }, { v5, n4 },
	{ v5, n5 }, { v11, n5 }, { v12, n5 },
	{ v6, n6 }, { v12, n6 }, { v7, n6 },
	{ v13, n7 }, { v14, n7 }, { v8, n7 },
	{ v8, n2 }, { v14, n2 }, { v15, n2 },
	{ v9, n8 }, { v15, n8 }, { v16, n8 },
	{ v16, n9 }, { v17, n9 }, { v11, n9 },
	{ v17, n5 }, { v18, n5 }, { v12, n5 },
	{ v18, n10 }, { v13, n10 }, { v7, n10 },
	{ v19, n11 }, { v20, n11 }, { v14, n11 },
	{ v20, n2 }, { v21, n2 }, { v15, n2 },
	{ v21, n12 }, { v22, n12 }, { v16, n12 },
	{ v22, n13 }, { v23, n13 }, { v17, n13 },
	{ v23, n5 }, { v24, n5 }, { v18, n5 },
	{ v24, n14 }, { v19, n14 }, { v13, n14 },
	{ v19, n15 }, { v25, n15 }, { v26, n15 },
	{ v26, n2 }, { v27, n2 }, { v21, n2 },
	{ v27, n16 }, { v28, n16 }, { v22, n16 },
	{ v28, n17 }, { v29, n17 }, { v23, n17 },
	{ v29, n5 }, { v30, n5 }, { v24, n5 },
	{ v30, n18 }, { v25, n18 }, { v19, n18 },
	{ v31, n19 }, { v32, n19 }, { v26, n19 },
	{ v32, n2 }, { v33, n2 }, { v27, n2 },
	{ v33, n20 }, { v34, n20 }, { v28, n20 },
	{ v34, n21 }, { v35, n21 }, { v29, n21 },
	{ v35, n5 }, { v36, n5 }, { v30, n5 },
	{ v36, n22 }, { v31, n22 }, { v25, n22 },
	{ v31, n23 }, { v37, n23 }, { v38, n23 },
	{ v32, n2 }, { v38, n2 }, { v39, n2 },
	{ v39, n24 }, { v40, n24 }, { v34, n24 },
	{ v40, n25 }, { v41, n25 }, { v35, n25 },
	{ v41, n5 }, { v42, n5 }, { v36, n5 },
	{ v42, n26 }, { v37, n26 }, { v31, n26 },
	{ v43, n3 }, { v44, n3 }, { v38, n3 },
	{ v44, n2 }, { v45, n2 }, { v39, n2 },
	{ v45, n1 }, { v46, n1 }, { v40, n1 },
	{ v46, n6 }, { v47, n6 }, { v41, n6 },
	{ v47, n5 }, { v48, n5 }, { v42, n5 },
	{ v48, n4 }, { v43, n4 }, { v37, n4 },
	{ v49, n8 }, { v50, n8 }, { v44, n8 },
	{ v44, n2 }, { v50, n2 }, { v51, n2 },
	{ v51, n7 }, { v52, n7 }, { v46, n7 },
	{ v52, n10 }, { v53, n10 }, { v47, n10 },
	{ v53, n5 }, { v54, n5 }, { v48, n5 },
	{ v48, n9 }, { v54, n9 }, { v49, n9 },
	{ v55, n12 }, { v56, n12 }, { v50, n12 },
	{ v56, n2 }, { v57, n2 }, { v51, n2 },
	{ v57, n11 }, { v58, n11 }, { v52, n11 },
	{ v58, n14 }, { v59, n14 }, { v53, n14 },
	{ v59, n5 }, { v60, n5 }, { v54, n5 },
	{ v60, n13 }, { v55, n13 }, { v49, n13 },
	{ v61, n16 }, { v62, n16 }, { v56, n16 },
	{ v62, n2 }, { v63, n2 }, { v57, n2 },
	{ v63, n15 }, { v64, n15 }, { v58, n15 },
	{ v64, n18 }, { v65, n18 }, { v59, n18 },
	{ v65, n5 }, { v66, n5 }, { v60, n5 },
	{ v66, n17 }, { v61, n17 }, { v55, n17 },
	{ v67, n20 }, { v68, n20 }, { v62, n20 },
	{ v68, n2 }, { v69, n2 }, { v63, n2 },
	{ v69, n19 }, { v70, n19 }, { v64, n19 },
	{ v64, n22 }, { v70, n22 }, { v71, n22 },
	{ v65, n5 }, { v71, n5 }, { v72, n5 },
	{ v72, n21 }, { v67, n21 }, { v61, n21 },
	{ v67, n24 }, { v1, n24 }, { v2, n24 },
	{ v68, n2 }, { v2, n2 }, { v3, n2 },
	{ v3, n23 }, { v4, n23 }, { v70, n23 },
	{ v4, n26 }, { v5, n26 }, { v71, n26 },
	{ v5, n5 }, { v6, n5 }, { v72, n5 },
	{ v72, n25 }, { v6, n25 }, { v1, n25 }
};