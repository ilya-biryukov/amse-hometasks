class Singleton
{
public:
    static Singleton & instance()
    {
        static Singleton instance;
        return instance;
    }

    int getTen()
    {
        return 10;          
    }
private:
    Singleton()
    {}
    ~Singleton()
    {};
    
    Singleton(const Singleton & );  
    Singleton& operator = (const Singleton &);      
};