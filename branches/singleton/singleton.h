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
    inline Singleton()
    {}   
    inline Singleton(const Singleton & )
    {}
    Singleton& operator = (const Singleton &)
    {}

};
