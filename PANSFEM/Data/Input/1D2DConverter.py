#***********************************************************
#Title      :1D2DConverter
#Author     :Tanabe Yuta
#Date       :2019/07/09
#Copyright  :(C)2019 TanabeYuta
#***********************************************************


import numpy as np


#**********Elementクラス**********
class Element(object):
    #----------ファイル情報から要素生成----------
    def __init__(self, _str):
        #節点設定
        self.Nodes = []             #要素を構成する節点の番号
        datas = _str.split(',')
        for i in range(int(datas[1])):
            self.Nodes.append(int(datas[i+2]))

        #辺のリスト
        self.Edges = []             #要素を構成する辺

        #追加の節点のリスト
        self.AdditionalNodes = []   #追加の節点番号


#**********二次元三角形要素**********
class Triangle(Element):
    def __init__(self, _str):
        super().__init__(_str)
        self.Edges.append([self.Nodes[0], self.Nodes[1]])
        self.Edges.append([self.Nodes[1], self.Nodes[2]])
        self.Edges.append([self.Nodes[2], self.Nodes[0]])
        for i in range(3):
            self.AdditionalNodes.append(None)


#**********二次元四角形要素**********
class Quadrangle(Element):
    def __init__(self, _str):
        super().__init__(_str)
        self.Edges.append([self.Nodes[0], self.Nodes[1]])
        self.Edges.append([self.Nodes[1], self.Nodes[2]])
        self.Edges.append([self.Nodes[2], self.Nodes[3]])
        self.Edges.append([self.Nodes[3], self.Nodes[0]])
        for i in range(4):
            self.AdditionalNodes.append(None)


#**********三次元六面体要素**********
class Cubic(Element):
    def __init__(self, _str):
        super().__init__(_str)
        self.Edges.append([self.Nodes[0], self.Nodes[1]])
        self.Edges.append([self.Nodes[1], self.Nodes[2]])
        self.Edges.append([self.Nodes[2], self.Nodes[3]])
        self.Edges.append([self.Nodes[3], self.Nodes[0]])
        self.Edges.append([self.Nodes[0], self.Nodes[4]])
        self.Edges.append([self.Nodes[1], self.Nodes[5]])
        self.Edges.append([self.Nodes[2], self.Nodes[6]])
        self.Edges.append([self.Nodes[3], self.Nodes[7]])
        self.Edges.append([self.Nodes[4], self.Nodes[5]])
        self.Edges.append([self.Nodes[5], self.Nodes[6]])
        self.Edges.append([self.Nodes[6], self.Nodes[7]])
        self.Edges.append([self.Nodes[7], self.Nodes[4]])
        for i in range(12):
            self.AdditionalNodes.append(None)
        

#**********メイン処理**********
file_path = "Optimize/AlternativeBeam/" #ファイルパス
DOF = 2                        #節点自由度
#NOA = 2                       #一辺辺りに追加する節点数

#----------ファイルから節点生成----------
nodes = []
with open(file_path + "Node.csv", 'r') as fp:
    fp.readline()
    while(True):
        line = fp.readline()
        if(not line):
            break
        nodes.append(np.array([float(line.split(',')[i+1]) for i in range(DOF)]))

#----------ファイルから要素生成----------
elements = []
with open(file_path + "Element.csv", 'r') as gp:
    gp.readline()
    while(True):
        line = gp.readline()
        if(not line):
            break
        elements.append(Quadrangle(line))

#----------節点の追加----------
for i in range(len(elements)):
    for ii, edgei in enumerate(elements[i].Edges):
        if(elements[i].AdditionalNodes[ii] == None):
            nid = len(nodes)
            #新しい節点を生成
            nodes.append(0.5*(nodes[edgei[0]] + nodes[edgei[1]]))
            elements[i].AdditionalNodes[ii] = nid
            for j in range(len(elements)):
                for jj, edgej in enumerate(elements[j].Edges):
                    if(set(edgei) == set(edgej)):
                        elements[j].AdditionalNodes[jj] = nid

#----------節点の出力----------
with open(file_path + "Node_modified.csv", 'w') as hp:
    hp.writelines("ID,x0,x1")
    for i, node in enumerate(nodes):
        hp.writelines("\n" + str(i))
        for x in node:
            hp.writelines("," + str(x))
            
#----------要素の出力----------
with open(file_path + "Element_modified.csv", 'w') as ip:
    ip.writelines("ID,NON")
    for i, element in enumerate(elements):
        ip.writelines("\n" + str(i) + "," + str(len(element.Nodes) + len(element.AdditionalNodes)))
        for node in (element.Nodes + element.AdditionalNodes):
            ip.writelines("," + str(node))
