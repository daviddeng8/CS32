void listAll(string path, const Class* c)  // two-parameter overload
{
    path = path + c->name();
    cout << path << endl;
    if (c->subclasses().size() == 0) {
        return;
    }
    path = path + "=>";
    for (int i = 0; i < c->subclasses().size(); ++i) {
        listAll(path, c->subclasses()[i]);
    }
}


