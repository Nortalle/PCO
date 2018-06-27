// VERSION NON VERFIFIED WITH NFOOD QUEUE

typedef enum {Pasta = 0, Rice = 1, Tomato = 2, NBFOODTYPES = 3} FoodType;

typedef struct {
	FoodType type;
} Food;

class FoodBuffer {
protected:
	Food array[][];
	QSemaphore waitProd;
	QSemaphore waitCons[];
	QSemaphore mutex;

	int size;
	int sizeMax;
	int writePointer[];
	int nbPerQueue[];
	int readPointer[];

	int nbWaitingConso[];
	int nbWaitingProd;

public:
	FoodBuffer(int sizeMax) : sizeMax(sizeMax), size(0) {
		mutex.release();

		array = new Food[NBFOODTYPES][sizeMax];
		waitCons = new QSemaphore[NBFOODTYPES];
		writePointer = new int[NBFOODTYPES]();
		readPointer = new int[NBFOODTYPES]();
		nbWaitingProd = 0;
		nbWaitingConso = new int[NBFOODTYPES]();
	}

	~FoodBuffer() {
		delete array;
		delete[] waitCons;
		delete[] writePointer;
		delete[] readPointer;
	}
	
	void put(Food food) {
		mutex.acquire();

		int type = food.type();	

		nbWaitingProd++;
		if(size >= sizeMax || nbPerQueue[type] >= sizeMax) {
			// We need to wait
			mutex.release();
			waitProd.acquire();

		}
		nbWaitingProd--;

		// critical area
		array[type][writePointer[type]] = food;
		writePointer[type] = (writePointer[type] + 1) % size; // current queue pointer
		++size; // total size
		++nbPerQueue[type];

		if(nbWaitingConso[type] > 0) {
			waitCons[type].release();
		} else {
			mutex.release();

		}
	}
	
	Food get(FoodType type) {
		mutex.acquire();

		nbWaitingConso[type]++;
		if(nbPerQueue[type] == 0) {
			// We need to wait
			mutex.release();
			waitCons[type].acquire();
		}
		nbWaitingConso[type]--;

		// critical area
		Food result = array[type][readPointer[type]];
		readPointer[type] = (readPointer[type] + 1) % size;
		--size;
		--nbPerQueue[type];

		if(nbWaitingProd > 0) {
			waitProd.release();
		} else {
			mutex.release();
		}

		return result;
	}
};




