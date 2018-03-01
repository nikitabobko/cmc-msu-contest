namespace numbers {
    class complex;

    class complex_stack {
        complex *stackPointer;
        size_t capacity;

        void assignTopStackElement(const complex &elem) {
            if (capacity == 0) {
                return;
            }
            stackPointer[capacity - 1] = elem;
        }

        void fillWithData(const complex_stack &stack) {
            size_t end = stack.size() < capacity ? stack.size() : capacity;
            for (size_t i = 0; i < end; i++) {
                stackPointer[i] = stack.stackPointer[i];
            }
        }

        void callConstructor(size_t capacity) {
            this->capacity = capacity;
            stackPointer = capacity == 0 ? nullptr : new complex[capacity];
        }

    public:
        complex_stack(size_t capacity = 0) {
            callConstructor(capacity);
        }

        complex_stack(const complex_stack &stack) {
            stackPointer = nullptr;
            *this = stack;
        }

        virtual ~complex_stack() {
            if (stackPointer == nullptr) {
                return;
            } 
            delete[] stackPointer;
        }

        complex_stack &operator=(const complex_stack &stack) {
            if (this == &stack) {
                return *this;
            }
            this->~complex_stack();
            callConstructor(stack.size());
            fillWithData(stack);
            return *this;
        }

        size_t size() const {
            return capacity;
        }

        complex operator[](size_t index) const {
            if (index >= capacity) {
                return complex();
            }
            return stackPointer[index];
        }

        friend complex_stack operator<<(const complex_stack &stack, const complex &elem) {
            complex_stack newStack(stack.size() + 1);
            newStack.fillWithData(stack);
            newStack.assignTopStackElement(elem);
            return newStack;
        }

        complex operator+() const {
            if (capacity == 0) {
                return complex();
            }
            return stackPointer[capacity - 1];
        }

        complex_stack operator~() const {
            complex_stack newStack(size() - 1);
            newStack.fillWithData(*this);
            return newStack;
        }
    };
}