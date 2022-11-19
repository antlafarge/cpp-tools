# Concurrent

## Create a concurrent resource

```c++
ConcurrentResource<std::string> concurrentResource("MyString");
```

## Get and use the readable resource

You can access the internal resource by using `readLock()`, and the `*` or `->` operators.  
The readable resource is `const` so you can't modify it directly (do a copy if you need to modify it locally).

```c++
auto readableResource = concurrentResource.readLock();
std::cout << "Resource = " << *readableResource << std::endl;
```

## Get and use the writable resource

You can access the internal resource by using `writeLock()`, and the `*` or `->` operators.  
The writable resource can be directly modified.

```c++
auto writableResource = concurrentResource.writeLock();
*writableResource = "MyStringHasChanged";
std::cout << "Resource = " << *writableResource << std::endl;
```

## Concurrent resource priority

There are multiple concurrent resource behaviors, default is `Queue`.

- `Queue`: Queue the operations.
- `Altern`: Altern between read and write operations.
- `Write`: Execute write operations after the next resource release.
- `Read`: Execute read operations after the next resource release.

Note: All waiting read operations are always executed simulaneously, and write operations are always executed one by one.
