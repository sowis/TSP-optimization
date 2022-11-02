# software_AI
2022-1 인공지능

## all_random
* 완전 무작위로 경로 생성
* 실행시 exe파일과 같은 위치에 TSP.csv 파일 필요함
* 결과 파일은 all_random.csv(경로 좌표 인덱스)
## all_GA
* 전체 노드(좌표)를 대상으로 유전 알고리즘을 사용해 경로를 찾음
* 옵션 선택은 main.cpp 주석 참조
* 실행시 exe파일과 같은 위치에 TSP.csv 파일 필요함
* 결과 파일은 all_GA.csv(경로 좌표 인덱스)
## cluster_random
* 랜덤하게 클러스터로 나눔
* 실행시 exe파일과 같은 위치에 TSP.csv 파일 필요함
* 결과 파일은 cluster_random.tsv
* 결과 파일은 경로 좌표 인덱스가 여러 줄로 된 파일인데, 각각의 행이 각각의 클러스터를 의미함(이하 클러스터를 나누는 프로그램 모두 동일)
## cluster_k_mean
* 클러스터를 k-평균법으로 나눔
* 실행시 exe파일과 같은 위치에 TSP.csv 파일 필요함
* 결과 파일은 cluster_kmeans.tsv
## cluster_mst
* 각 클러스터별 최소 스패닝 트리의 경로 길이 합이 가장 작게 되도록 클러스터를 나눔
* 실행시 exe파일과 같은 위치에 TSP.csv 파일 필요함
* 결과파일은 cluster_mst.tsv
## cluster_hamilton
* 각 클러터별로 길이가 가장 짧은 해밀턴 순환 길이의 합이 가장 작게 되도록 클러스터를 나눔
* 실행시 exe파일과 같은 위치에 TSP.csv 파일 필요함
* 결과파일은 cluster_hamilton.tsv
* 다른 클러스터링 프로그램에 비해서 계산량이 많기 때문에 좀 느림
## GA_cluster
* 클러스터들의 순서를 유전 알고리즘을 이용하여 배열
* 실행시 exe파일과 같은 위치에 TSP.csv 파일, 클러스터 파일 필요함
* 사용법: this.exe 클러스터파일이름
* 결과파일은 ga_cluster.csv(경로 좌표 인덱스)
* 프로그램 실행시 전처리에 5분정도 걸림

## visualization
* .py 파일이라 터미널에서 opencv가 설치된 상황에서 python visualization.py 실행
* 결과가 원활이 안나오는 경우 주피터 노트북을 설치하여
* 주피터 노트북에서 새 파일 생성한 상태에서 visualization.py 코드 붙여넣기
* visualization.ipynb (주피터 노트북 .py) 는 용량 문제(github <25MB)로 업로드 못함
