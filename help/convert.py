normals = """"""
"""
arr_nrm = normals.split("\n")

split_nrm = [x.split("vn ") for x in arr_nrm]

split_nrm = [x[1:] for x in split_nrm]
# print(split_nrm)

nrm_verts = [[n.split(" ")[:] for n in item] for item in split_nrm]
# print(nrm_verts)

for num in range(len(nrm_verts)):
    print("const vertex n"+str(num+1)+" = {"+nrm_verts[num][0][0]+", "+nrm_verts[num][0][1]+", "+nrm_verts[num][0][2]+"};")
    # print("const float t"+str(num+1)+"[2] = {"+nrm_verts[num][0][0]+", "+nrm_verts[num][0][1]+"};")
"""

obj = """"""

arr_obj = obj.split("\n")
# print(arr_obj)

obj_split = [x.split(" ") for x in arr_obj]
# print(obj_split)

obj_split = [x[1:] for x in obj_split]
# print(obj_split)

obj_vertecies = [[vertex.split("/")[0] for vertex in item]for item in obj_split]
# print(obj_vertecies)

# obj_texcoords = [[vertex.split("/")[1] for vertex in item]for item in obj_split]

obj_normals = [[vertex.split("/")[2] for vertex in item]for item in obj_split]
# print(obj_normals)

for num in range(len(obj_vertecies)):
    # print("\t{ v"+obj_vertecies[num][0]+", t"+obj_texcoords[num][0]+", n"+obj_normals[num][0]+" }, { v"+obj_vertecies[num][1]+", t"+obj_texcoords[num][1]+", n"+obj_normals[num][1]+" }, { v"+obj_vertecies[num][2]+", t"+obj_texcoords[num][2]+", n"+obj_normals[num][2]+" },")
    print("\t{ v"+obj_vertecies[num][0]+", n"+obj_normals[num][0]+" }, { v"+obj_vertecies[num][1]+", n"+obj_normals[num][1]+" }, { v"+obj_vertecies[num][2]+", n"+obj_normals[num][2]+" },")
print(len(obj_vertecies*3))

print("};")