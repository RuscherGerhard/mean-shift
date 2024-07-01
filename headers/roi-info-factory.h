
typedef struct RoiInfo {
    unsigned long cols;
    unsigned long rows;
    unsigned long start_row;
    unsigned long end_row;
    unsigned long start_col;
    unsigned long end_col;
    unsigned int cn;
    bool bad;
} _RoiInfo;

class RoiInfoFactory{

    public:
    // C-Tor
    RoiInfoFactory(unsigned long cols, unsigned long rows, unsigned int cn);
    
    // D-tor
    ~RoiInfoFactory();

    RoiInfo create(unsigned long start_row, unsigned long end_row, unsigned long start_col, unsigned long end_col);

    private:
    unsigned long cols;
    unsigned long rows;
    unsigned long channels; 

    bool sanityChecks(unsigned long end_row, unsigned long end_col);
};