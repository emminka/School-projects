# BIN projekt
# Vehicle Routing Problem s kapacitou
# Emma Krompascikova
# xkromp00
# LS 2023/2024


import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
import math
import random
import copy
import heapq

FLEET = 3
CAPACITY = 40 
COEFICIENT_DISTANCE = 0
COEFICIENT_TIME = 1
X_VALUES = []
Y_VALUES = []


class Car:
    def __init__(self, index, capacity):
        self.index = index
        self.capacity = capacity
        
#distance je celkova prejdena cesta vsetkymi autami
#time je vzdialenost auta, ktore ide najdlhsiu cestu
#flotila je pocet aut
#podla velkosti koeficinetov vieme nastavit prioritu
def fitness_function(distance,time,fleet):
    fitness = ((COEFICIENT_DISTANCE*distance/fleet)+COEFICIENT_TIME*time)
    return fitness

def calculate_total_distance(assigned_nodes):
    total_distance = 0
    if assigned_nodes:
        total_distance += adj_matrix[0][assigned_nodes[0]] #z depa do prveho uzlu
        for i in range(len(assigned_nodes) - 1): #medzi uzlami
            node1 = assigned_nodes[i]
            node2 = assigned_nodes[i + 1]
            total_distance += adj_matrix[node1][node2]
        total_distance += adj_matrix[assigned_nodes[-1]][0] #z poslednej naspat do depa
    return total_distance

#dijkstrov algortimus pre najdenie najkratsej cesty z nejeho uzlu
def dijkstra(graph, source): 
    num_vertices = len(graph)
    distances = [math.inf] * num_vertices
    distances[source] = 0
    visited = [False] * num_vertices

    for _ in range(num_vertices):
        current_vertex = get_minimum_distance_vertex(distances, visited)
        visited[current_vertex] = True
        for neighbor in range(num_vertices):
            if not visited[neighbor] and graph[current_vertex][neighbor] != 0 and distances[current_vertex] + graph[current_vertex][neighbor] < distances[neighbor]:
                distances[neighbor] = distances[current_vertex] + graph[current_vertex][neighbor]
    return distances


def get_minimum_distance_vertex(distances, visited):
    min_distance = math.inf
    min_vertex = -1
    for i in range(len(distances)):
        if not visited[i] and distances[i] < min_distance:
            min_distance = distances[i]
            min_vertex = i
    return min_vertex

#vypocet celkovej vzdialenosti kazdeho auta prvykrat
def compute_distance(car_node_assignment, best_solutions):
    for car_index, assigned_nodes in car_node_assignment.items():
        total_distance = calculate_total_distance(assigned_nodes)
        car_total_distance[car_index] = total_distance

    sum_distance=0
    all_distances = []
    for car_index, distance in car_total_distance.items():
        all_distances.append(distance)
        sum_distance = sum_distance+distance

    max_distance = max(all_distances)

    #pre najdenie vzdialenosti bez ohladu na fitness
    #current_solution = (sum_distance, car_node_assignment)
    
    current_solution = (fitness_function(sum_distance,max_distance,FLEET), car_node_assignment)
    best_solutions.append(current_solution)
            
#vypocet celkovej vzdialenosti kazdeho auta v generaciach
def compute_distance_simple(car_node_assignment):
    mut_total_distance = {}
    for car_index, assigned_nodes in car_node_assignment.items():
        total_distance = calculate_total_distance(assigned_nodes)
        mut_total_distance[car_index] = total_distance

    sum_distance=0
    all_distances = []
    for car_index, distance in mut_total_distance.items():
        all_distances.append(distance)
        sum_distance = sum_distance+distance
    
    max_distance = max(all_distances)
    
    #pre najdenie vzdialenosti bez ohladu na fitness
    #current_solution = (sum_distance, car_node_assignment)
    
    current_solution = (fitness_function(sum_distance,max_distance,FLEET), car_node_assignment)
    
    return current_solution

#pre urobenie mutacie
# s 50-percentnou sancou sa uzol prehodi do ineho auta s volnou kapacitou  
# s 50-percentnou sancou sa uzol prehodi v ramci jedneho auta (napr. z [2,5,6] bude [2,6,5] )
def make_mutation(best_solutions, capacity):
    mutated_solutions = []

    sorted_list = sorted(best_solutions, key=lambda x: x[0])
    for item in sorted_list:
            mutated_solutions.append(item)
            #print(item)
            break
    
    for solution in best_solutions:
        mutated_assignment = copy.deepcopy(solution[1])
        for i in range(len(mutated_assignment)):
            if random.random() < 0.5: #medzi autami
                car_indices = list(mutated_assignment.keys())
                random.shuffle(car_indices)
                for car_index in car_indices:
                    assigned_nodes = mutated_assignment[car_index]
                    if assigned_nodes:  #ci nie je prazdny
                        node_to_switch = random.choice(assigned_nodes)
                        available_cars = [target_car_index for target_car_index, target_nodes in mutated_assignment.items() if target_car_index != car_index and (sum(value_order_dict[node] for node in target_nodes) + (value_order_dict.get(node_to_switch, 0) if node_to_switch is not None else 0)) <= capacity]
                        if available_cars:
                            target_car_index = random.choice(available_cars)
                            mutated_assignment[car_index].remove(node_to_switch)
                            mutated_assignment[target_car_index].append(node_to_switch)
                            break  #aby sa presunul len 1 uzol
            else: #vramci auta
                car_index = random.choice(list(mutated_assignment.keys()))
                assigned_nodes = mutated_assignment[car_index]
                if len(assigned_nodes) > 1:  #na prehodenie musia byt aspon 2 uzly
                    pos1, pos2 = random.sample(range(len(assigned_nodes)), 2)
                    assigned_nodes[pos1], assigned_nodes[pos2] = assigned_nodes[pos2], assigned_nodes[pos1]
        
        new_dist = compute_distance_simple(mutated_assignment)
        mutated_solutions.append(new_dist)
    
    return mutated_solutions
    
    

def keep_five_best(long_assignment):
    #usporiadaj na zaklade prveho
    sorted_list = sorted(long_assignment, key=lambda x: x[0])
    #necham prvych 5
    result = sorted_list[:5]
    return result

def print_first(long_assignment):
    global  Y_VALUES
    sorted_list = sorted(long_assignment, key=lambda x: x[0])
    #print(sorted_list)
    for item in sorted_list:
            Y_VALUES.append(item[0])
            #print(item[0])
            break


# pre kazdu generaciu necham z predchadzajucej 25 najlepsich genov a zvysne 
# nahodne premiesam a necham si z nich polovicu
def twentyfive_best_and_50_percent_random(long_assignment):

 
    unique_solutions = {}

    for distance, solution in long_assignment:
        solution_key = tuple((k, tuple(v)) for k, v in solution.items())
        #print("SULU KEY", solution_key)
        
        # proti opakovaniu sa
        if solution_key not in unique_solutions:
            unique_solutions[solution_key] = (distance, solution)
    
    # usporiadavam na zaklade vzdialenosti (fitness)
    sorted_solutions = sorted(unique_solutions.values(), key=lambda x: x[0])
    #print(sorted_solutions)
    
    #nechavam si 25
    best_solution = sorted_solutions[:25]
    #print("BEST 25",best_solution)
    
    #zvysok zamiesam
    remaining_solutions = sorted_solutions[25:]
    random.shuffle(remaining_solutions)
    #necham si polovicu
    num_to_keep = int(len(remaining_solutions) * 0.4)
    selected_solutions = remaining_solutions[:num_to_keep]
    result = best_solution + selected_solutions

    
    

    
    return result


#urcena pozicia uzlom
# 0 beriem ako depo
node_positions = {
    0: (0, 0),
    1: (14, -26),
    2: (-6, -4),
    3: (22, -36),
    4: (-8, -24),
    5: (18, 18),
    6: (30, 1),
    7: (-4, 35),
    8: (-27, 36),
    9: (-30, -4),
    10: (35, -7),
    11: (36, -30),
    12: (-4, 26),
    13: (17, -14),
    14: (5, 10),
    15: (-15, 15),
}

#vytvorenie grafu
G = nx.complete_graph(16)

cars = []


#vypocitanie vzdialenosti medzi depom a kazdym uzlom 
depot_position = node_positions[0]
edge_labels = {}
for node in range(1, 16):
    node_position = node_positions[node]
    distance = math.sqrt((node_position[0] - depot_position[0])**2 + (node_position[1] - depot_position[1])**2)
    edge_labels[(0, node)] = f"{distance:.0f}" #pre vypisanie do grafu s 0 desatinnymi miestami

# mnou vytvorene prepojenia medzi mestami (uzlami)
distances = {}
adj_matrix = np.zeros((16, 16))
adj_matrix[7][12] = 10
adj_matrix[12][7] = 10

adj_matrix[5][11] = 21
adj_matrix[11][5] = 21

adj_matrix[3][11] = 11
adj_matrix[11][3] = 11

adj_matrix[1][13] = 8
adj_matrix[13][1] = 8

adj_matrix[1][3] = 12
adj_matrix[3][1] = 12

adj_matrix[3][4] = 31
adj_matrix[4][3] = 31

adj_matrix[8][9] = 22
adj_matrix[9][8] = 22

adj_matrix[2][1] = 32
adj_matrix[1][2] = 32

adj_matrix[12][15] = 19
adj_matrix[15][12] = 19

adj_matrix[9][15] = 11
adj_matrix[15][9] = 11

adj_matrix[8][15] = 25
adj_matrix[15][8] = 25

adj_matrix[14][6] = 20
adj_matrix[6][14] = 20

adj_matrix[6][5] = 6
adj_matrix[5][6] = 6

adj_matrix[7][15] = 18
adj_matrix[15][7] = 18

adj_matrix[6][10] = 9
adj_matrix[10][6] = 9

adj_matrix[5][14] = 22
adj_matrix[14][5] = 22


#pridanie vzdialenosti do matice sousednosti
for node1 in range(16):
    for node2 in range(16):
        distance = 0
        if node1 != node2:
            if node1 == 0 or node2 == 0:
                #jednoduchy vypocet pre spojenie s depom
                distance += math.sqrt((node_positions[node1][0] - node_positions[0][0])**2 + (node_positions[node1][1] - node_positions[0][1])**2)
                distance += math.sqrt((node_positions[node2][0] - node_positions[0][0])**2 + (node_positions[node2][1] - node_positions[0][1])**2)
                adj_matrix[node1][node2] = distance
            else:
                #pouzitie dijkstrovho algoritmu pre najdenie najrkatsej vzdialenosti medzi uzlami
                if adj_matrix[node1][node2] == 0:
                   distances = dijkstra(adj_matrix, node1)
                   adj_matrix[node1][node2] = distances[node2]

# pouzivam mnou vybrany seed aby malo kazde spustenie rovnaku nahodnost
seed_value = 15
random.seed(seed_value)
random_numbers = [random.randint(0, 20) for _ in range(16)]
value_order_dict = {}

# priradujem kazdemu uzlu jeho nahodnu hodnotu 
# moze byt aj nula - to mesto otom nebudem musiet autom navstivit
for i, uzol in enumerate(G.nodes()):
    if uzol != 0: 
        G.nodes[uzol]['hodnota_objednavky'] = random_numbers[i]
        value_order_dict[uzol] = random_numbers[i]

best_solutions = []

# jedna flotila je jeden jedinec
# vytvaram 10 pociatocnych jedincov
# do flotily su nahodne priradovane uzly, ale kontroluje sa,
# aby sa nepresiahla kapacita auta
specimen = 0
while specimen < 10:
    cars = [Car(index=i, capacity=CAPACITY) for i in range(FLEET)]
    car_node_assignment = {car.index: [] for car in cars}

    for uzol in G.nodes():
        if uzol == 0:
            continue
        
        order_value = value_order_dict[uzol]
        
        if order_value == 0:  #preskakujem mesta s nulovou nodnotou
            continue
        # najdem dostupne auta s dostatocnou kapacitou
        available_cars = [car for car in cars if car.capacity >= order_value]
        if available_cars:
            assigned_car = random.choice(available_cars)
            assigned_car.capacity -= order_value
            car_node_assignment[assigned_car.index].append(uzol)
        else:
            print(f"No available cars")

    car_total_distance = {}
    car_total_distance = compute_distance(car_node_assignment,best_solutions)
    specimen = specimen+1

generation = 1
while generation<1001:
    mutated_solu = make_mutation(best_solutions,CAPACITY)
    new_mix = best_solutions + mutated_solu



    

    best_solutions = []
    
    # do generacie 8 nechavam vsetky povodne aj zmutovane moznosti
    # od generacie 9 necham stale 25 najlepsich a nahodnu polovicu zvysku
    if generation >8:
        best_solutions = twentyfive_best_and_50_percent_random(new_mix)
        for item in best_solutions:
            Y_VALUES.append(item[0])
            break
        #print(best_solutions)
        

    else:
        best_solutions = new_mix
        #print(best_solutions)
        print_first(best_solutions)
    
    #
        
        
    #na pomocne vypisanie aby osm vedela ako rychlo to ide  
    '''if generation == 1:
       # print("generation", generation)
        sortt_list = sorted(best_solutions, key=lambda x: x[0])
        #print(sorted_list)
        for item in sortt_list:
            print(item)
            break

    if generation == 50000:
       # print("generation", generation)
       for item in best_solutions:
        print(item)
        break'''

    generation = generation+1

best_solutions = keep_five_best(new_mix)
#print("FINAL GEN ",generation,"je", best_solutions)



# aby som mohla vypisovat so vzdielenostami na 0 desatinnych miest
np.set_printoptions(precision=0)
#print(adj_matrix)

# vykreslenie grafu pre dane pozicie uzlov a hrany
nx.draw(G, pos=node_positions, with_labels=False, node_color='lightblue', font_weight='bold',node_size=800)

# v zavtorke v grafe je hodnota kazdeho uzla (okrem depa)
node_labels = {node: f"{node} ({G.nodes[node].get('hodnota_objednavky', '')})" for node in G.nodes() if node != 0}  # Exclude the depot node
nx.draw_networkx_labels(G, pos=node_positions, labels=node_labels,font_size=8)

# pre vypisanie vzdialenosti od depa na hranach
nx.draw_networkx_edge_labels(G, pos=node_positions, edge_labels=edge_labels)

# Display the graph
plt.show()
generation_numbers = range(1,1001)
plt.plot(generation_numbers, Y_VALUES)
#print(Y_VALUES)
plt.ylim(ymin=0)  # this line
plt.xlabel('Generation')
plt.ylabel('Specific Numbers')
plt.title('Line Graph with Generation Numbers')

# Show the plot
plt.show()
print(Y_VALUES)

