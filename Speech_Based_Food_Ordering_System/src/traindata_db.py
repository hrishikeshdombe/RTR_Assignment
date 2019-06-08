from pymongo import MongoClient

if __name__ == "__main__":
    con = MongoClient()

    db = con.traindata_database

    data = db.train_data

    #data.insert({'Train_Data' : 'I want to order a Pizza', 'Start_Char' : '18', 'End_Char' : '23', 'Entity' : 'Veg Pizza'})
    """data.insertMany([{'Train_Data' : 'I want to order a Cheese Pizza', 'Start_Char' : (18,25), 'End_Char' : (24,30), 'Entity' : ('Pizza Type','Pizza')},
                     {'Train_Data' : 'I want to order a Farmhouse Pizza', 'Start_Char' : (18,28), 'End_Char' : (27,33), 'Entity' : ('Pizza Type','Pizza')},
                     {'Train_Data' : 'I want to order a Double Cheese Margherita Pizza', 'Start_Char' : (18,43), 'End_Char' : (42,48), 'Entity' : ('Pizza Type','Pizza')},
                     {'Train_Data' : 'I want to order a Burger Pizza Classic Veg', 'Start_Char' : (18), 'End_Char' : (42), 'Entity' : ('Burger Pizza')},
                     {'Train_Data' : 'Hey my name is Hrishikesh and I want to order a Burger Pizza Premium Veg and Coke in California', 'Start_Char' : (48,77), 'End_Char' : (72,81), 'Entity' : ('Burger Pizza','Beverages')},
                     {'Train_Data' : 'I want to order two Cheese and Corn Pizzas', 'Start_Char' : (16,20,36), 'End_Char' : (19,35,42), 'Entity' : ('Quantity','Pizza Type','Pizza')}    
                    ])"""

    data.insert({'Train_Data' : 'order margherita pizza', 'Start_Char' : (6,17), 'End_Char' : (16,22), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I want to order a Farmhouse Pizza', 'Start_Char' : (18,28), 'End_Char' : (27,33), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I want to order a Double Cheese Margherita Pizza', 'Start_Char' : (18,43), 'End_Char' : (42,48), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'extravaganza pizza', 'Start_Char' : (1,14), 'End_Char' : (13,19), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I like double cheese margherita pizza order me one', 'Start_Char' : (7,32), 'End_Char' : (31,37), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I want to order two Cheese and Corn Pizzas', 'Start_Char' : (16,20,36), 'End_Char' : (19,35,42), 'Entity' : ('Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I like farmhouse pizza do you have one if there order it for me', 'Start_Char' : (7,17), 'End_Char' : (16,22), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I have heard that Mexican green wave is nice Pizza so I would like to order 1 Mexican green wave pizza', 'Start_Char' : (76,78,97), 'End_Char' : (77,96,102), 'Entity' : ('Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'Paneer Makhani Pizza I love it I want to order one', 'Start_Char' : (0,15,48), 'End_Char' : (14,20,51), 'Entity' : ('Pizza Type','Pizza','Quantity')})
    data.insert({'Train_Data' : 'I want one peppy paneer pizza', 'Start_Char' : (7,11,24), 'End_Char' : (10,23,29), 'Entity' : ('Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'wow I love chicken sausage Pizza I would like to have one', 'Start_Char' : (11,27,54), 'End_Char' : (26,32,57), 'Entity' : ('Pizza Type','Pizza','Quantity')})
    data.insert({'Train_Data' : 'do you have Paneer Makhani pizza I want to order it', 'Start_Char' : (12,27), 'End_Char' : (26,32), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I would like to order chicken feast pizza', 'Start_Char' : (22,36), 'End_Char' : (35,41), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'yeah I would like to order a non veg Supreme Pizza chicken sausage pizza and fresh veggie pizza', 'Start_Char' : (29,45,55,68), 'End_Char' : (44,50,67,73), 'Entity' : ('Pizza Type','Pizza','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I would like to have one farmhouse pizza', 'Start_Char' : (21,25,35), 'End_Char' : (24,34,40), 'Entity' : ('Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'peppy paneer pizza and pepper barbeque and onion pizza', 'Start_Char' : (0,13,23,49), 'End_Char' : (12,18,48,54), 'Entity' : ('Pizza Type','Pizza Type','Pizza Type','Pizza Type')})
    data.insert({'Train_Data' : 'peppy paneer pizza', 'Start_Char' : (0,13), 'End_Char' : (12,18), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'chicken sausage pizza', 'Start_Char' : (0,16), 'End_Char' : (15,31), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'pepper Barbecue chicken pizza', 'Start_Char' : (0,24), 'End_Char' : (23,29), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'chicken Fiesta pizza', 'Start_Char' : (0,15), 'End_Char' : (14,20), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'chicken golden delight pizza', 'Start_Char' : (0,23), 'End_Char' : (22,28), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'non veg supreme pizza', 'Start_Char' : (0,16), 'End_Char' : (15,21), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I love Pizza so I would like to have double cheese margherita pizza and 2 Mexican green wave pizza', 'Start_Char' : (37,62,72,74,93), 'End_Char' : (61,67,73,92,98), 'Entity' : ('Pizza Type','Pizza','Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'OK I will love to order margherita pizza and 3 Deluxe veggie pizza', 'Start_Char' : (24,35,45,47,61), 'End_Char' : (34,40,46,60,66), 'Entity' : ('Pizza Type','Pizza','Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I love Pizza I would like to order extravaganza pizza also like to order one more pizza order me chicken sausage Pizza', 'Start_Char' : (35,48,97,105), 'End_Char' : (47,53,104,110), 'Entity' : ('Pizza Type','Pizza','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'ok I will order a pepper Barbecue chicken pizza', 'Start_Char' : (18,41), 'End_Char' : (40,46), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'yes sure I would like to have three margherita pizza and 2 cheese and corn Pizza', 'Start_Char' : (30,36,47,57,59,75), 'End_Char' : (35,46,52,58,74,80), 'Entity' : ('Quantity','Pizza Type','Pizza','Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'OK let me think what should I order ok ok I will try Mexican green wave pizza', 'Start_Char' : (53,72), 'End_Char' : (71,77), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'Yeah sure yes yes I will get an extravaganza pizza', 'Start_Char' : (32,45), 'End_Char' : (44,50), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'you know my friend told me your veg Paradise is very good order veg Paradise pizza please', 'Start_Char' : (64,77), 'End_Char' : (76,82), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'yes do you have chicken Fiesta pizza order 3 for me', 'Start_Char' : (16,31,43), 'End_Char' : (30,36,44), 'Entity' : ('Pizza Type','Pizza','Quantity')})
    data.insert({'Train_Data' : 'margherita pizza', 'Start_Char' : (0,11), 'End_Char' : (10,16), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'double cheese margherita pizza', 'Start_Char' : (0,25), 'End_Char' : (24,30), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'farmhouse pizza', 'Start_Char' : (0,10), 'End_Char' : (9,15), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'peppy paneer pizza', 'Start_Char' : (0,13), 'End_Char' : (12,18), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'Mexican green wave pizza', 'Start_Char' : (0,19), 'End_Char' : (18,24), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'extravaganza pizza', 'Start_Char' : (0,13), 'End_Char' : (12,18), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'cheese and corn pizza', 'Start_Char' : (0,16), 'End_Char' : (15,21), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'Paneer Makhani pizza', 'Start_Char' : (0,15), 'End_Char' : (14,20), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'veggie Paradise pizza', 'Start_Char' : (0,16), 'End_Char' : (15,21), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'fresh veggie pizza', 'Start_Char' : (0,13), 'End_Char' : (12,18), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : '2 Pepper barbeque and onion pizza and 3 farmhouse pizza', 'Start_Char' : (0,2,28,38,40,50), 'End_Char' : (1,27,33,39,49,55), 'Entity' : ('Quantity','Pizza Type','Pizza','Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : '2 Mexican green wave pizza and 3 Deluxe veggie pizza', 'Start_Char' : (0,2,21,31,33,47), 'End_Char' : (1,20,26,32,46,52), 'Entity' : ('Quantity','Pizza Type','Pizza','Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'one fresh veggie pizza and five paneer Makhani pizza', 'Start_Char' : (0,4,13,23,29,43), 'End_Char' : (3,12,18,27,42,48), 'Entity' : ('Quantity','Pizza Type','Pizza','Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : '3 pepper Barbecue chicken pizza 1 peppy paneer and 3 chicken Dominator pizza', 'Start_Char' : (0,2,26,32,34,52), 'End_Char' : (1,25,31,33,51,57), 'Entity' : ('Quantity','Pizza Type','Pizza','Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'you know I love eating pizza I would like to have farmhouse pizza today', 'Start_Char' : (50,60), 'End_Char' : (59,65), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I heard that you have new pizza on menu order cheese and corn pizza for me', 'Start_Char' : (46,62), 'End_Char' : (61,67), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'yes I have heard that you have excellent quality of Pizza so I have also heard that you have good chicken sausage Pizza yes I would like to order one chicken sausage pizza', 'Start_Char' : (48,52,67), 'End_Char' : (51,66,52), 'Entity' : ('Quantity','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I would like to have chicken Dominator pizza', 'Start_Char' : (21,39), 'End_Char' : (38,44), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'OK get a non veg supreme pizza', 'Start_Char' : (9,25), 'End_Char' : (24,30), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'Mexican green wave pizza and veggie Paradise pizza', 'Start_Char' : (0,19,28,44), 'End_Char' : (18,24,43,49), 'Entity' : ('Pizza Type','Pizza','Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'you know I always like to eat pizza today i want to eat fresh veggie pizza', 'Start_Char' : (56,69), 'End_Char' : (68,74), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'I had always wanted to have a farmhouse pizza please order it for me', 'Start_Char' : (30,40), 'End_Char' : (39,45), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'you know I will love to have a chicken Fiesta pizza', 'Start_Char' : (31,46), 'End_Char' : (45,51), 'Entity' : ('Pizza Type','Pizza')})
    data.insert({'Train_Data' : 'actually last time I had pepper Barbecue chicken and it was good so I will have a pepper Barbecue chicken pizza', 'Start_Char' : (82,106), 'End_Char' : (105,111), 'Entity' : ('Pizza Type','Pizza')})

    item = data.find()

    for i in item:
        print(i)


    """item = data.find_one({'Start_Char' : (18,25)})
    print(type(item))

    print("*********************")
    print(item)
    print("*********************")
    print(item['Train_Data'])"""

    """for n in item:
        print(n['Train_Data'])
        print(n['Start_Char'])
        print(n['End_Char'])
        print(n['Entity'])"""

    """print("*********************")
    item = data.find_one({'Start_Char' : (18,25)})
    if item is not None:
        length = len(item['Start_Char'])
        l = list()
        for i in range(length):
            t = (item['Start_Char'][i],item['End_Char'][i],item['Entity'][i])
            l.append(t)
        print(l)
        print(length)
        d = dict()
        d["entities"] = l
        l = [item['Train_Data'],d]
        t = tuple(l)
        l = list()
        l.append(t)
        print(l)"""

    item = data.find()

    k = list()

    for i in item:
        if i is not None:
            print("*********************")
            try:
                length = len(i['Start_Char'])
            except TypeError:
                length = 1
                print(length)
            l = list()
            if length is 1:
                t = (i['Start_Char'],i['End_Char'],i['Entity'])
                l.append(t) 
            else:
                for j in range(length):
                    t = (i['Start_Char'][j],i['End_Char'][j],i['Entity'][j])
                    l.append(t)
            #print(l)
            #print(length)
            d = dict()
            d["entities"] = l
            l = [i['Train_Data'],d]
            t = tuple(l)
            k.append(t)
            print(k)

    """for item in data.find():
        data.remove(item)"""

