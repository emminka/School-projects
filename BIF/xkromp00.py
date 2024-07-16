#Emma Krompascikova
#xkromp00
#BIF projekt 2021/2022
#script bezi na mojom pocitaci cca 40sekund
import csv
from Bio import SeqIO
from Bio import Phylo

msa = []
tree = Phylo.read("tree.tre", "newick")  #nacitanie struktury tree
terminaly = tree.get_terminals() #zoznam listovych uzlov
neterminaly = tree.get_nonterminals() #zoznam internych uzlov aka nodov
pomocny_slovnicek = {}
pomocny_list_na_slovnicek = []
aktualna_noda = 0

with open('ancestrals.csv') as f: #nacitanie ancestrals
    ancenstral = [{k: v for k, v in row.items()}
        for row in csv.DictReader(f, skipinitialspace=True)]

for seq_record in SeqIO.parse("msa.fasta", "fasta"): #nacitanie viacnasobneho zarovnania z msa.fasta
    msa.append(seq_record)

for i in ancenstral:  #for cyklus na zapisanie najpravdepodobnejsich animokyselin ku kazdej node do pomocneho slovnicka
    cislo_nody_kde_som = i.get('node')  
    if cislo_nody_kde_som != aktualna_noda:
        aktualna_noda = cislo_nody_kde_som
        pomocny_slovnicek[cislo_nody_kde_som] = []
    for key in i:
        if i[key] == '-':
            i[key] = 0 
        else:
            i[key] = float(i[key])    
    del i['node']
    del i['position']
    maximum_zo_vsetkych = max(i, key=i.get)
    pomocny_list_na_slovnicek.append(maximum_zo_vsetkych)
    pomocny_slovnicek[cislo_nody_kde_som] += [str(maximum_zo_vsetkych)]

for node_konkretny in neterminaly: #prechadzam vsetky nody   
    iba_ciselko =  str(node_konkretny.confidence) #cislo nodu kde som
    nazov_file = 'node_' + iba_ciselko + '.fas'
    f = open(nazov_file, 'w') #vytvorenie file-u
    for pozicia_cislo in range(len(pomocny_slovnicek[aktualna_noda])): #prechadzam kazdu poziciu
        je_to_pomlcka = 0
        neni_to_pomlcka = 0
        for list_konkretny in terminaly: #prechadzam vsetky listove uzly
            dlzka_od_nodu_k_listu = tree.distance(list_konkretny,node_konkretny)
            je_rodicom = node_konkretny.is_parent_of(list_konkretny) #zistujem, ci je node rodicom listu
            if (je_rodicom == True): 
                for anc_seq in msa: #prechadzam sekvencie zo suboru msa.fasta          
                    if anc_seq.name == str(list_konkretny):
                        if (anc_seq.seq[pozicia_cislo]) == '-': #zistujem ci je na pozicii medzera
                            je_to_pomlcka = je_to_pomlcka + dlzka_od_nodu_k_listu
                        else:
                            neni_to_pomlcka = neni_to_pomlcka + dlzka_od_nodu_k_listu
        if je_to_pomlcka > neni_to_pomlcka: #vyberanie, ci sa do file zapise medzera alebo konkretna aminokyselina
            print('-', end="", file=f)
        else:
            print(pomocny_slovnicek[iba_ciselko][pozicia_cislo], end="", file=f)
