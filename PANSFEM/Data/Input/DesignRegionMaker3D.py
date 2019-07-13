#*******************************************************************************
#Title      :DesignRagionMaker3D
#Author     :Tanabe Yuta
#Data       :2019/07/10
#Copyright  :(C)2019 TanabeYuta
#*******************************************************************************

fname = "Beam"

LX = [2.0, 3.0, 4.0]      #辺長さ
NX = [2, 3, 4]            #要素数
DX = [LX[0]/NX[0], LX[1]/NX[1], LX[2]/NX[2]]     #要素の辺長さ

Nodes = []          #節点座標のリスト
Elements = []       #要素のリスト

#----------節点の追加----------
for k in range(NX[2] + 1):
    for j in range(NX[1] + 1):
        for i in range(NX[0] + 1):
            Nodes.append([i*DX[0], j*DX[1], k*DX[2]])

#----------要素の追加----------
for i in range(NX[0]):
    for j in range(NX[1]):
        for k in range(NX[2]):
            Elements.append([(NX[0]+1)*(NX[1]+1)*k+(NX[0]+1)*j+i, (NX[0]+1)*(NX[1]+1)*k+(NX[0]+1)*j+(i+1), (NX[0]+1)*(NX[1]+1)*k+(NX[0]+1)*(j+1)+(i+1), (NX[0]+1)*(NX[1]+1)*k+(NX[0]+1)*(j+1)+i, \
                             (NX[0]+1)*(NX[1]+1)*(k+1)+(NX[0]+1)*j+i, (NX[0]+1)*(NX[1]+1)*(k+1)+(NX[0]+1)*j+(i+1), (NX[0]+1)*(NX[1]+1)*(k+1)+(NX[0]+1)*(j+1)+(i+1), (NX[0]+1)*(NX[1]+1)*(k+1)+(NX[0]+1)*(j+1)+i])

#----------節点をファイルに出力----------
fp = open(fname + "_Node.csv", 'w')
fp.write("ID,x0,x1,x2")
for i in range(len(Nodes)):
    fp.write("\n" + str(i) + "," + str(Nodes[i][0]) + "," + str(Nodes[i][1]) + "," + str(Nodes[i][2]))
fp.close()

#----------要素をファイルに出力----------
gp = open(fname + "_Element.csv", 'w')
gp.write("ID,NON,n0,n1,n3,n4,n5,n6,n7,n8")
for i in range(len(Elements)):
    gp.write("\n" + str(i) + ",8," + str(Elements[i][0]) + "," + str(Elements[i][1]) + "," + str(Elements[i][2]) + "," + str(Elements[i][3]) + "," + str(Elements[i][4]) + "," + str(Elements[i][5]) + "," + str(Elements[i][6]) + "," + str(Elements[i][7]))
gp.close()
