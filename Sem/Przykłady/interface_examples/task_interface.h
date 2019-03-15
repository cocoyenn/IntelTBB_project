namespace tbb {
        class task {
        protected:
            task();
     
        public:
            virtual ~task() {}
     
            virtual task* execute() = 0;
     
            // Allocation
            static proxy1 allocate_root();
            static proxy2 allocate_root( task_group_context& );
            proxy3 allocate_continuation();
            proxy4 allocate_child();
            static proxy5 allocate_additional_child_of( task& );
     
            // Explicit destruction
            static void destroy( task& victim );
     
            // Recycling
            void recycle_as_continuation();
            void recycle_as_safe_continuation();
            void recycle_as_child_of( task& new_parent );
     
            // Synchronization
            void set_ref_count( int count );
            void increment_ref_count();
            int add_ref_count( int count );
            int decrement_ref_count();
            void wait_for_all();
            static void spawn( task& t );
            static void spawn( task_list& list );
            void spawn_and_wait_for_all( task& t );  
            void spawn_and_wait_for_all( task_list& list );  
            static void spawn_root_and_wait( task& root );
            static void spawn_root_and_wait( task_list& root );
            static void enqueue( task& );
     
            // Task context
            static task& self();
            task* parent() const;
            void set_parent( task *p );
            bool is_stolen_task() const;
            task_group_context* group();
            void change_group( task_group_context& ctx );
     
            // Cancellation
            bool cancel_group_execution();
            bool is_cancelled() const;
     
            // Priority
            static void enqueue( task&, priority_t );
            void set_group_priority ( priority_t );
            priority_t group_priority () const;
     
            // Affinity
            typedef implementation-defined-unsigned-type affinity_id;
            virtual void note_affinity( affinity_id id );
            void set_affinity( affinity_id id );
            affinity_id affinity() const;
     
            // Debugging
            enum state_type {
                executing,
                reexecute,
                ready,
                allocated,
                freed
            };
            int ref_count() const;
            state_type state() const;
        };
    } // namespace tbb
     
    void *operator new( size_t bytes, const proxy1& p );
    void operator delete( void* task, const proxy1& p );
    void *operator new( size_t bytes, const proxy2& p );
    void operator delete( void* task, const proxy2& p );
    void *operator new( size_t bytes, const proxy3& p );
    void operator delete( void* task, const proxy3& p );
    void *operator new( size_t bytes, proxy4& p );
    void operator delete( void* task, proxy4& p );
    void *operator new( size_t bytes, proxy5& p );
    void operator delete( void* task, proxy5& p );