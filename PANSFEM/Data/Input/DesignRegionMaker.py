#*******************************************************************************
#Title      :DesignRagionMaker
#Author     :Tanabe Yuta
#Date       :2019/07/07
#Copyright  :(C)2019 TanabeYuta
#*******************************************************************************

fname = "Beam"

LX = [375.0, 250.0]     #辺長さ
NX = [150, 100]         #要素数
DX = [LX[0]/NX[0], LX[1]/NX[1]]     #要素の辺長さ

Nodes = []          #節点座標のリスト
Elements = []       #要素のリスト

#----------節点の追加----------
for i in range(NX[0] + 1):
    for j in range(NX[1] + 1):
        Nodes.append([i*DX[0], j*DX[0]])

#----------要素の追加----------
for i in range(NX[0]):
    for j in range(NX[1]):
        Elements.append([i*(NX[1]+1)+j, (i+1)*(NX[1]+1)+j, (i+1)*(NX[1]+1)+(j+1), i*(NX[1]+1)+(j+1)])

#----------節点をファイルに出力----------
fp = open(fname + "_Node.csv", 'w')
fp.write("ID,x0,x1")
for i in range(len(Nodes)):
    fp.write("\n" + str(i) + "," + str(Nodes[i][0]) + "," + str(Nodes[i][1]))
fp.close()

#----------要素をファイルに出力----------
gp = open(fname + "_Element.csv", 'w')
gp.write("ID,NON,n0,n1,n3,n4")
for i in range(len(Elements)):
    gp.write("\n" + str(i) + ",4," + str(Elements[i][0]) + "," + str(Elements[i][1]) + "," + str(Elements[i][2]) + "," + str(Elements[i][3]))
gp.close()
