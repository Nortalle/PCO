#include <QSemaphore>

/** 
 * HEIG-TIC-PCO TE2 2015 Ex.23 (c) YTA Yann Thoma
 * Implementation: QSemaphore
 * Version: acknowledge in class.
 */
typedef enum {Pasta = 0, Rice = 1, Tomato = 2, NBFOODTYPES = 3} FoodType;

typedef struct {
	FoodType type;
} Food;

class FoodBuffer {
protected:
	QSemaphore mutex;
	QSemaphore waitProd; // single wait line for produder
	int prodWaiting; // producer count
	
	QSemaphore waitCons[NBFOODTYPES]; // wait lines for types of food
	int consWaiting[NBFOODTYPES]; // nb people waiting fot each lines of types
	Food *array; // internal buffer
	int size; // buffer size
	int writePointer, readPointer, nbTot; // read & write pointer, nb of item stored
public:
	FoodBuffer(int size) : size(size), writePointer(0), readPointer(0), nbTot(0) {
		mutex.release();
		array = new Food[size];
		prodWaiting = 0;
		for(int i = 0; i < NBFOODTYPES; ++i) {
			consWaiting[i] = 0;
		}
	}
	
	void put(Food food) {
		// prelude
		mutex.acquire();
		if (nbTot == size) {
			prodWaiting++;
			mutex.release();
			waitProd.acquire(); // mutex reception (1)
		}

		// critical area
		array[writePointer] = food;
		writePointer = (writePointer + 1) % size;
		nbTot++;

		// post-lude
		// NOT OK: if (consWaiting[food.type] > 0) {
		// ?!?: if (consWaiting[food.type] > 0 && food.type == array[readPointer].type) {
		if (consWaiting[array[readPointer].type] > 0) {
			consWaiting[food.type] --;
			waitCons[food.type].release(); // mutex transmission (2)
		} else {
			mutex.release();
		}
	}
	
	Food get(FoodType type) {
		Food result;

		// prelude
		mutex.acquire();
		// if there is nothing or if it's not the correct type
		if ((nbTot == 0) || (array[readPointer].type != type)) {
			consWaiting[type]++;
			mutex.release();
			waitCons[type].acquire(); // mutex reception (2)
		}

		// critical area
		result = array[readPointer];
		readPointer = (readPointer + 1) % size;
		nbTot--;

		// post-lude
		if (consWaiting[array[readPointer].type] > 0) {
			// if someone wants to eat what is there, then go for it
			consWaiting[food.type] --;
			waitCons[food.type].release(); // mutex transmission (2)
		} else if(prodWaiting > 0) {
			// if there are productors waiting
			prodWaiting --;
			waitProd.release(); // mutex transmission (1)
		} else {
			mutex.release();
		}
		
		return result;
	}
	
};