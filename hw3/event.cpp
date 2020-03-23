class Event {
public:
    Event(string name): m_name(name) {}
    string name() const {
        return m_name;
    }
    virtual bool isSport() const { //spec specifies that most events are sporting events
        return true;
    }
    virtual string need() const = 0;
    virtual ~Event() {

    }
    
private:
    string m_name;
};

class BasketballGame: public Event {
public:
    BasketballGame(string name): Event(name) {}
    
    string need() const {
        return "hoops";
    }
    
    ~BasketballGame() {
        cout << "Destroying the " << name() << " basketball game" << endl;
    }
    
private:
};

class Concert: public Event {
public:
    Concert(string name, string genre): Event(name), m_genre(genre) {}
    
    bool isSport() const {
        return false;
    }
    
    string need() const {
        return "a stage";
    }
    
    ~Concert() {
        cout << "Destroying the " << name() << " " << m_genre <<  " concert" << endl;
    }
    
private:
    string m_genre;
};

class HockeyGame: public Event {
public:
    HockeyGame(string name): Event(name) {}
    
    bool isSport() const {
        return true;
    }
    
    string need() const {
        return "ice";
    }
    
    ~HockeyGame() {
        cout << "Destroying the " << name() << " hockey game" << endl;
    }
};




