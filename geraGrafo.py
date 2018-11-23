import random as rd

n = input("Vertices: ")
edges = rd.randint(1, 2*n)
print("Max arestas: %d" % edges)

file = open("grafoGerado.txt", "w")

file.write(str(n)+"\n")

for i in range(1, n+1):
    node = str(i)+" "+str(rd.randrange(1, 100))+"\n"
    file.write(node)

addedEdges = []

for i in range(edges):
    node1 = rd.randint(1, n)
    node2 = rd.randint(1, n)
    while(node2 == node1):
        node2 = rd.randint(1, n)
    edgeEl1 = (node1, node2)
    edgeEl2 = (node2, node1)
    if(edgeEl1 in addedEdges or edgeEl2 in addedEdges):
        continue
    addedEdges.append(edgeEl1)
    addedEdges.append(edgeEl2)
    weight = rd.randrange(1, 50)
    edge = str(node1)+" "+str(node2)+" "+str(weight)+"\n"
    file.write(edge)

file.close()
