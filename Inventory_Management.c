#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
};

void inputProductDetails(struct Product *product) {
    printf("Enter Product ID: ");
    scanf("%d", &product->id);
    printf("Enter Product Name: ");
    scanf("%s", product->name);
    printf("Enter Product Price: ");
    scanf("%f", &product->price);
    printf("Enter Product Quantity: ");
    scanf("%d", &product->quantity);
}
void displayProductDetails(struct Product *product){
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", product->id, product->name, product->price, product->quantity);
}

void addNewProduct(struct Product *products, int *size){
    *size += 1;
    products = realloc(products, (*size) * sizeof(struct Product));
    
    printf("Enter new product details:\n");
    inputProductDetails(&products[*size - 1]);
    printf("Product added successfully!\n");
}

void viewAllProducts(struct Product *products, int size){
    printf("\n========= Product List =========\n");
    for(int i = 0; i < size; i++){
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", products[i].id, products[i].name, products[i].price, products[i].quantity);
    }
    printf("\n");
}

void updateProductQuantity(struct Product *products, int size){
    int id,newQuantity;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);
    printf("Enter new Quantity: ");
    scanf("%d", &newQuantity);
    for(int i=0;i<size;i++){
        if(products[i].id == id){
            products[i].quantity = newQuantity;
            printf("Quantity updated successfully!\n");
            return;
        }
    }
    
    
}

struct Product * searchProductByID(struct Product *products, int size, int id){
    if(id==0){
        printf("Enter Product ID to search: ");
        scanf("%d", &id);
    }
    for(int i=0;i<size;i++){
        if(products[i].id == id){
            return &products[i];
        }
    }
    return NULL;
}

void searchProductByName(struct Product *products, int size){
    char name[50];
    printf("Enter name to search (partial allowed): ");
    scanf("%s", name);
    for(int i=0;i<size;i++){
        if(strstr(products[i].name, name) != NULL){
            displayProductDetails(&products[i]);
            return;
        }
    }
}

void searchProductsByPriceRange(struct Product *products, int size){
    float minPrice, maxPrice;
    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);
    printf("Products in price range:\n");
    for(int i=0;i<size;i++){
        if(products[i].price >= minPrice && products[i].price <= maxPrice){
            displayProductDetails(&products[i]);
        }
    }
}

void deleteProductByID(struct Product *products, int *size){
    int id;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);
    struct Product *product = searchProductByID(products, *size, id);
    if(product!= NULL){
        int index = product - products;
        for(int i=index;i<*size-1;i++){
            products[i] = products[i+1];
        }
        *size -= 1;
        products = realloc(products, (*size) * sizeof(struct Product));
        printf("Product deleted successfully!\n");
    }
    
}


int main(){
    int numProducts, choice;
    printf("Enter initial number of products: ");
    scanf("%d", &numProducts);
    struct Product *products= calloc(numProducts, sizeof(struct Product));

    for(int i = 0; i < numProducts; i++){
        printf("Enter details for product %d:\n", i + 1);
        inputProductDetails(&products[i]);
        printf("\n");
    }

    while(1){
        printf("\n========= Inventory Menu =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Products by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\n");
        switch(choice){
            case 1:
                addNewProduct(products, &numProducts);
                break;
            case 2:
                viewAllProducts(products, numProducts);
                break;
            case 3:
                updateProductQuantity(products, numProducts);
                break;
            case 4:
                products=searchProductByID(products, numProducts, 0);
                displayProductDetails(products);
                break;
            case 5:
                searchProductByName(products, numProducts);
                break;
            case 6:
                searchProductsByPriceRange(products, numProducts);
                break;
            case 7:
                deleteProductByID(products, &numProducts);
                break;
            case 8:
                free(products);
                printf("Memory released successfully. Exiting program...");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}