struct Test {
    int a;
    char b;
    union {
        double a;
        float b;
    } c;
};

int main() {
    const char *str = "abcdefghijk";

    for (int a = 0; a < 10; ++a) {
        if (a == 0)
            continue;

        if (a % 2 == 1)
            break;

        print(str[a]);
    }

    Test test = {1, 2, 3};
    while (true) {
        print(test.a);
        print((&test)->c.a);
    }

    switch (a) {
    case 1:
        break;

    case 2:
        break;

    default:
        break;
    }
}
