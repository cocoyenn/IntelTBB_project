
#if TBB_USE_ASSERT
    // We cannot use __TBB_ASSERT as it is because it calls a function from tbb library which may
    // be not yet loaded. Redefine __TBB_ASSERT not to call tbb functions.
    #undef __TBB_ASSERT
    #define __TBB_ASSERT( cond, msg ) {                                                            \
        if ( ! (cond) ) {                                                                          \
            say( "%s:%d: Assertion failed: %s.", __FILE__, __LINE__, (msg) );                      \
        }                                                                        \
    }
#endif

namespace tbb {

namespace internal {

// blocked_rangeNd_impl forward declaration in tbb::internal namespace to
// name it as a friend for a tbb::blocked_range.
template<typename Value, unsigned int N, typename>
class blocked_rangeNd_impl;

} 

//! A range over which to iterate.
template<typename Value>
class blocked_range {
public:
    //! Type of a value
    /** Called a const_iterator for sake of algorithms that need to treat a blocked_range
        as an STL container. */
    typedef Value const_iterator;

    //! Type for size of a range
    typedef std::size_t size_type;

    //...#if __TBB_DEPRECATED_BLOCKED_RANGE_DEFAULT_CTOR -> blocked_range() : my_end(), my_begin(), my_grainsize() {}

    //! Construct range over half-open interval [begin,end), with the given grainsize.
    blocked_range( Value begin_, Value end_, size_type grainsize_=1 ) :
        my_end(end_), my_begin(begin_), my_grainsize(grainsize_)
    {
        __TBB_ASSERT( my_grainsize>0, "grainsize must be positive" );
    }

    //! Beginning of range.
    const_iterator begin() const {return my_begin;}

    //! One past last value in range.
    const_iterator end() const {return my_end;}

    //! Size of the range
    /** Unspecified if end()<begin(). */
    size_type size() const {
        __TBB_ASSERT( !(end()<begin()), "size() unspecified if end()<begin()" );
        return size_type(my_end-my_begin);
    }

    //! The grain size for this range.
    size_type grainsize() const {return my_grainsize;}

    //------------------------------------------------------------------------
    // Methods that implement Range concept
    //------------------------------------------------------------------------

    //! True if range is empty.
    bool empty() const {return !(my_begin<my_end);}

    //! True if range is divisible.
    /** Unspecified if end()<begin(). */
    bool is_divisible() const {return my_grainsize<size();}

    //! Split range.
    /** The new Range *this has the second part, the old range r has the first part.
        Unspecified if end()<begin() or !is_divisible(). */
    blocked_range( blocked_range& r, split ) :
        my_end(r.my_end),
        my_begin(do_split(r, split())), //my_begin(do_split(r, proportion)),
        my_grainsize(r.my_grainsize)
    {
        // only comparison 'less than' is required from values of blocked_range objects
        __TBB_ASSERT( !(my_begin < r.my_end) && !(r.my_end < my_begin), "blocked_range has been split incorrectly" );
    }

    //...#if __TBB_USE_PROPORTIONAL_SPLIT_IN_BLOCKED_RANGES
    //   blocked_range( blocked_range& r, proportional_split& proportion ) :

private:
    /** NOTE: my_end MUST be declared before my_begin, otherwise the splitting constructor will break. */
    Value my_end;
    Value my_begin;
    size_type my_grainsize;

    //! Auxiliary function used by the splitting constructor.
    static Value do_split( blocked_range& r, split )
    {
        __TBB_ASSERT( r.is_divisible(), "cannot split blocked_range that is not divisible" );
        Value middle = r.my_begin + (r.my_end - r.my_begin) / 2u;
        r.my_end = middle;
        return middle;
    }

    //...#if __TBB_USE_PROPORTIONAL_SPLIT_IN_BLOCKED_RANGES static Value do_split( blocked_range& r, proportional_split& proportion )
    
    template<typename RowValue, typename ColValue>
    friend class blocked_range2d;

    template<typename RowValue, typename ColValue, typename PageValue>
    friend class blocked_range3d;

    template<typename DimValue, unsigned int N, typename>
    friend class internal::blocked_rangeNd_impl;
};


template<typename RowValue, typename ColValue=RowValue>
class blocked_range2d {
public:
    //! Type for size of an iteration range
    typedef blocked_range<RowValue> row_range_type;
    typedef blocked_range<ColValue> col_range_type;

private:
    row_range_type my_rows;
    col_range_type my_cols;

public:

    blocked_range2d( RowValue row_begin, RowValue row_end, typename row_range_type::size_type row_grainsize,
                     ColValue col_begin, ColValue col_end, typename col_range_type::size_type col_grainsize ) :
        my_rows(row_begin,row_end,row_grainsize),
        my_cols(col_begin,col_end,col_grainsize)
    {}

    blocked_range2d( RowValue row_begin, RowValue row_end,
                     ColValue col_begin, ColValue col_end ) :
        my_rows(row_begin,row_end),
        my_cols(col_begin,col_end)
    {}

    //! True if range is empty
    bool empty() const {
        return my_rows.empty() || my_cols.empty();
    }

    //! True if range is divisible into two pieces.
    bool is_divisible() const {
        return my_rows.is_divisible() || my_cols.is_divisible();
    }

    blocked_range2d( blocked_range2d& r, split ) :
        my_rows(r.my_rows),
        my_cols(r.my_cols)
    {
        split split_obj;
        do_split(r, split_obj);
    }

    //! The rows of the iteration space
    const row_range_type& rows() const {return my_rows;}

    //! The columns of the iteration space
    const col_range_type& cols() const {return my_cols;}

private:

    template <typename Split>
    void do_split( blocked_range2d& r, Split& split_obj )
    {
        if( my_rows.size()*double(my_cols.grainsize()) < my_cols.size()*double(my_rows.grainsize()) ) {
            my_cols.my_begin = col_range_type::do_split(r.my_cols, split_obj);
        } else {
            my_rows.my_begin = row_range_type::do_split(r.my_rows, split_obj);
        }
    }
};


template<typename PageValue, typename RowValue=PageValue, typename ColValue=RowValue>
class blocked_range3d {
public:
    //! Type for size of an iteration range
    typedef blocked_range<PageValue> page_range_type;
    typedef blocked_range<RowValue>  row_range_type;
    typedef blocked_range<ColValue>  col_range_type;

private:
    page_range_type my_pages;
    row_range_type  my_rows;
    col_range_type  my_cols;

public:

    blocked_range3d( PageValue page_begin, PageValue page_end,
                     RowValue  row_begin,  RowValue row_end,
                     ColValue  col_begin,  ColValue col_end ) :
        my_pages(page_begin,page_end),
        my_rows(row_begin,row_end),
        my_cols(col_begin,col_end)
    {}

    blocked_range3d( PageValue page_begin, PageValue page_end, typename page_range_type::size_type page_grainsize,
                     RowValue  row_begin,  RowValue row_end,   typename row_range_type::size_type row_grainsize,
                     ColValue  col_begin,  ColValue col_end,   typename col_range_type::size_type col_grainsize ) :
        my_pages(page_begin,page_end,page_grainsize),
        my_rows(row_begin,row_end,row_grainsize),
        my_cols(col_begin,col_end,col_grainsize)
    {}

    //! True if range is empty
    bool empty() const {
        // Range is empty if at least one dimension is empty.
        return my_pages.empty() || my_rows.empty() || my_cols.empty();
    }

    //! True if range is divisible into two pieces.
    bool is_divisible() const {
        return  my_pages.is_divisible() || my_rows.is_divisible() || my_cols.is_divisible();
    }

    blocked_range3d( blocked_range3d& r, split ) :
        my_pages(r.my_pages),
        my_rows(r.my_rows),
        my_cols(r.my_cols)
    {
        split split_obj;
        do_split(r, split_obj);
    }

    //! The pages of the iteration space
    const page_range_type& pages() const {return my_pages;}

    //! The rows of the iteration space
    const row_range_type& rows() const {return my_rows;}

    //! The columns of the iteration space
    const col_range_type& cols() const {return my_cols;}

private:
    template <typename Split>
    void do_split( blocked_range3d& r, Split& split_obj)
    {
        if ( my_pages.size()*double(my_rows.grainsize()) < my_rows.size()*double(my_pages.grainsize()) ) {
            if ( my_rows.size()*double(my_cols.grainsize()) < my_cols.size()*double(my_rows.grainsize()) ) {
                my_cols.my_begin = col_range_type::do_split(r.my_cols, split_obj);
            } else {
                my_rows.my_begin = row_range_type::do_split(r.my_rows, split_obj);
            }
        } else {
            if ( my_pages.size()*double(my_cols.grainsize()) < my_cols.size()*double(my_pages.grainsize()) ) {
                my_cols.my_begin = col_range_type::do_split(r.my_cols, split_obj);
            } else {
                my_pages.my_begin = page_range_type::do_split(r.my_pages, split_obj);
            }
        }
    }
};