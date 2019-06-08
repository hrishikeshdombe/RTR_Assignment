from pymongo import MongoClient

if __name__ == "__main__":
	con = MongoClient()

	db = con.pizza_database

	menu = db.menu

	menu.insert({'Pizza' : 'Margherita', 'Quantity' : '10' , 'Price' : '300'})
	menu.insert({'Pizza' : 'Double Cheese Margherita', 'Quantity' : '10' , 'Price' : '400'})
	menu.insert({'Pizza' : 'Farm House', 'Quantity' : '10' , 'Price' : '450'})
	menu.insert({'Pizza' : 'Peppy Paneer', 'Quantity' : '10' , 'Price' : '465'})
	menu.insert({'Pizza' : 'Mexican Green Wave', 'Quantity' : '10' , 'Price' : '450'})
	menu.insert({'Pizza' : 'Deluxe Veggie', 'Quantity' : '10' , 'Price' : '420'})
	menu.insert({'Pizza' : 'Extravaganza', 'Quantity' : '10' , 'Price' : '500'})
	menu.insert({'Pizza' : 'Cheese and Corn', 'Quantity' : '10' , 'Price' : '400'})
	menu.insert({'Pizza' : 'Paneer Makhani', 'Quantity' : '10' , 'Price' : '430'})
	menu.insert({'Pizza' : 'Veggie Paradise', 'Quantity' : '10' , 'Price' : '425'})
	menu.insert({'Pizza' : 'Fresh Veggie', 'Quantity' : '10' , 'Price' : '450'})
	menu.insert({'Pizza' : 'Pepper Barbeque Chicken', 'Quantity' : '10' , 'Price' : '520'})
	menu.insert({'Pizza' : 'Chicken Sausage', 'Quantity' : '10' , 'Price' : '560'})
	menu.insert({'Pizza' : 'Chicken Golden Delight', 'Quantity' : '10' , 'Price' : '580'})
	menu.insert({'Pizza' : 'Non Veg Supreme', 'Quantity' : '10' , 'Price' : '590'})
	menu.insert({'Pizza' : 'Chicken Dominator', 'Quantity' : '10' , 'Price' : '600'})
	menu.insert({'Pizza' : 'Pepper Barbeque and Onion', 'Quantity' : '10' , 'Price' : '620'})
	menu.insert({'Pizza' : 'Chicken Fiesta', 'Quantity' : '10' , 'Price' : '610'})
	"""menu.insert({'Side Orders' : 'Garlic Breadsticks', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Stuffed Garlic Bread', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Veg Pasta Italiano White', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Non Veg Pasta Italiano White', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Cheese Jalapeno Dip', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Cheese Dip', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Lava Cake', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Butterscotch Mouse Cake', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Potato Cheese Shots', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Crunchy Strips', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Taco Mexicana Veg', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Taco Mexicana Non Veg', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Crinkle Fries', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Veg Parcel', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Chicken Parcel', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Side Orders' : 'Brownie Fantasy', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Choice of Crusts' : 'Cheese Burst', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Choice of Crusts' : 'Classic Hand Tossed', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Choice of Crusts' : 'Wheat Thin Crust', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Choice of Crusts' : 'Fresh Pan Pizza', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Choice of Crusts' : 'New Hand Tossed', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Choice of Crusts' : 'Multigrain', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Burger Pizza' : 'Burger Pizza Classic Veg', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Burger Pizza' : 'Burger Pizza Premium Veg', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Burger Pizza' : 'Burger Pizza Classic Non Veg', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Chicken' : 'Roasted Chicken Wings Peri Peri', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Chicken' : 'Roasted Chicken Wings Classic Hot Sauce', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Chicken' : 'Chicken Meatballs Peri Peri', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Chicken' : 'Boneless Chicken Wings Peri Peri', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Beverages' : 'Lipton Ice Tea', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Beverages' : 'Mirinda', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Beverages' : 'Pepsi', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Beverages' : 'Water Bottle', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})
	menu.insert({'Beverages' : '7 Up', 'Quantity' : '10' , 'Price' : '400' , 'Price_Large' : '600'})"""
	
	item = menu.find()
	
	print ("INSERT & FIND TEST")
	for i in item:
		print(i)
		
	"""peeps = people.find({'food':'cheese'})
	
	print ("FIND WITH DICT TEST")
	for person in peeps:
		print(person)
		
	peeps = people.find({'name' :{'$regex': '.*[Mm]i.*'}})
	
	print ("REGEX TEST")
	for person in peeps:
		print(person)
		
	person = people.find_one({'food':'ham'})
	person['food'] = 'eggs'
	people.save(person)
	
	print ("UPDATE RECORD TEST")
	for person in people.find({'food' : 'eggs'}):
		print(person)"""
		
	for item in menu.find():
		menu.remove(item)
		
