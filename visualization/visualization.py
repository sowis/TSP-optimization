# terminal에서 jupyter notebook 으로 실행하거나
# terminal에서 'python visualization.py'로 직접 실행
# TSP.csv랑 all_ga.csv, ga_cluster, ..., 등 파일이 visualization.py 폴더에 있어야 실행가능
# In[1]:


# https://colab.research.google.com/github/Albina1810/tsp/blob/master/TSP.ipynb#scrollTo=ICeg6g5uq_Tc

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import csv
import cv2

# 유클리디안 거리 계산
# path_distance = lambda r,c: np.sum([np.linalg.norm(c[int(r[p])]-c[int(r[p-1])]) for p in range(len(r))])

def calculate_distance(city_1,city_2):
    dist = np.linalg.norm(np.array(city_1) - np.array(city_2))
    return dist

def calculate_total_distance(sol, cities):
    total_cost = 0
    for idx in range(len(sol)-1) :
        pos_city_1 = [float(cities[int(sol[idx])][0]), float(cities[int(sol[idx])][1])]
        pos_city_2 = [float(cities[int(sol[idx+1])][0]), float(cities[int(sol[idx+1])][1])]  
        dist = calculate_distance(pos_city_1, pos_city_2)
        total_cost += dist
    return total_cost

def calculate_now_distance(sol, cities):
    dist = []
    for idx in range(len(sol)-1) :
        pos_city_1 = [float(cities[int(sol[idx])][0]), float(cities[int(sol[idx])][1])]
        pos_city_2 = [float(cities[int(sol[idx+1])][0]), float(cities[int(sol[idx+1])][1])]  
        dist.append(calculate_distance(pos_city_1, pos_city_2))
    return dist


# In[2]:

# route = 경로 솔루션 = index, scaled = (x,y) 값

df = pd.read_csv("TSP.csv", header=None);
scaled = np.asarray(df)
#csv
# route = pd.read_csv("all_random.csv", header=None);                     # path_distance: 51958.61248152609
route_df = pd.read_csv("all_ga.csv", header=None);                         # path_distance: 6744.073867773974
# route = pd.read_csv("ga_cluster.csv", headerNone);                      # path_distance: 6896.57078606587

route = np.array(route_df)
route = route.ravel()

sorted_scaled = df
sorted_scaled["route"] = route_df
sorted_scaled.sort_values("route", inplace=True)
sorted_scaled = np.array(sorted_scaled)


# In[3]:


# 노드 방문 순서 정확하지 않아 index 정렬하고 순서 구함

new_cities_order = sorted_scaled

plt.figure(figsize=(100, 100))
plt.scatter(scaled[:,0],scaled[:,1], s=1000)
plt.plot(new_cities_order[:,0],new_cities_order[:,1])
plt.show()
# print("\n\npath_distance: " + str(calculate_total_distance(route,scaled)))
print("Route: " + str(route.ravel()) + "\n\npath_distance: " + str(calculate_total_distance(route,scaled)))


# In[4]:

mul = 18 # 노드가 너무 가까이 붙어있으면 관측하기 힘들어서 배율 확대
move = 250
display_size = (3360, 2100)
ratio = (0.85, 1)
display = (int(display_size[0]*ratio[0]), int(display_size[1]*ratio[1])) #display_size를 세부 조정

total = 0
max_node = 1000

map_original = cv2.imread('white.png')
map_original = cv2.resize(map_original, display_size)

for i in range(0, max_node):
    map_result = map_original.copy()
    map_result = cv2.resize(map_result, display_size)
    dist = calculate_now_distance(route, scaled)
    
    x1, y1 = (int(sorted_scaled[i][0])*mul + move, int(scaled[i][1])*mul + move)

    if i > 0:    #거리는 두개의 노드가 있어야 하나가 생성. i==1일때 0번째, 1번쨰 노드가 생성.
        total += dist[i-1] #dist[0 ~ 999]
    if i <= max_node-2:  #i+1 == 999. 즉, 마지막 노드이후에는 out of index 이므로
        x2, y2 = (int(sorted_scaled[i+1][0])*mul + move, int(scaled[i+1][1])*mul + move)
        
        cv2.circle(map_original, center=(x1, y1), radius=8, color=(0, 0, 255), thickness=-1, lineType=cv2.LINE_AA)
        cv2.line(
            map_original,
            pt1=(x1, y1),
            pt2=(x2, y2),
            color=(200, 0, 0),
            thickness=1,
            lineType=cv2.LINE_AA
        )
    else : # 마지막 노드 scatter 처리
        cv2.circle(map_original, center=(x2, y2), radius=8, color=(0, 0, 255), thickness=-1, lineType=cv2.LINE_AA)
    
    cv2.putText(map_result, org=(display[0], 50), text="node : %d counts" % (i+1), fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=1, color=0, thickness=1, lineType=cv2.LINE_AA)
    cv2.putText(map_result, org=(display[0], 80), text='Distance: %.2f' % dist[i-1], fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=1, color=0, thickness=1, lineType=cv2.LINE_AA)    
    cv2.putText(map_result, org=(display[0], 110), text='Now Distance: %.2f' % total, fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=1, color=0, thickness=1, lineType=cv2.LINE_AA)
    cv2.putText(map_result, org=(display[0], 140), text='Total Distance: %.2f' % calculate_total_distance(route, scaled), fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=1, color=0, thickness=1, lineType=cv2.LINE_AA)
    cv2.putText(map_result, org=(display[0], 170), text='multiplied size: x%.2f' % mul, fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=1, color=0, thickness=1, lineType=cv2.LINE_AA)
    
    cv2.imshow('map', map_result)
    cv2.waitKey(1)
#     cv2.destroyAllWindows()
# cv2.imshow('map', map_result)