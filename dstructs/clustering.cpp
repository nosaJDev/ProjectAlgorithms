#include "clustering.hpp"
#include <cstdlib>
#include <cstdio>

LloydClusterer::LloydClusterer(int d, List * pts, int _k,bool range,PointStruct * q){

    // This will initialize the basic data structures involved in clustering
    
    // Initialize the parameters
    k = _k;
    rangeAssign = range;
    query = q;
    points = pts;
    dimension = d;
    queryDone = false;

    // Initialize the cluster hash table
    clusters = new HashTable(k);

    // Initialize the centroids array and fill it with placeholder vectors
    centroids = new Vector*[k];
    for(int i = 0; i < k; i++){
        centroids[i] = new Vector(d);
    }

    // Initialize the sillouete array
    sillouete = new float[k];

}

LloydClusterer::~LloydClusterer(){

    // Delete the centroids
    for(int i = 0; i < k; i++){
        delete centroids[i];
    }
    delete[] centroids;

    // Delete the query object if it exists
    if(query != nullptr)
        delete query;

    // Delete the clusters hashtable
    delete clusters;

    // Delete the silouetes
    delete[] sillouete;


}

void LloydClusterer::initialization(Metric * metric){

    // This will initialize the centroids using the k-means++ method

    int els = points->getElems();

    // How many centroids do you have so far
    int t = 0;

    // Put all the List elements in an array temporarily
    Vector ** pts = new Vector*[els];
    for(int i = 0; i < els; i++){
        pts[i] = (Vector *) points->get(i);
    }

    // Initialize the minimum distance array
    float * dists = new float[els];
    float distsum = 0;

    // Pick the first centroid at random
    int pick = rand() % els;

    // Replace and repick for the following centroids
    while(true){

        // Find and replace the next centroid
        centroids[t++]->paste(pts[pick]);
        Vector * temp = pts[pick];
        pts[pick] = pts[els-t];
        pts[els-t] = temp;

        // Stop if you reached your wanted centroids
        if (t >= k) break;

        // Recalculate the distances for the rest of the points
        distsum = 0;
        for(int i = 0; i < els-t; i++){

            // First minimum is the first centroid
            float d = metric->dist(centroids[0],pts[i]);
            dists[i] = d*d;

            for(int j = 1; j < t; j++){
                // Check if there is a closer one
                d = metric->dist(centroids[j],pts[i]);
                if(d*d < dists[i])
                    dists[i] = d*d;
            }

            // Add to the distance sum
            distsum += dists[i];

        }

        // Then pick one at random with proportionate probability
        float probdist = distsum*rand()/RAND_MAX;

        // Check which one you picked
        float tempdist = 0;
        for(int i = 0; i < els-t; i++){
            tempdist += dists[i];
            if (tempdist >= probdist){
                // You found your pick
                pick = i;
                break;
            }
        }
    }

    // After that, you will have gathered all the centroids
    // Delete the leftovers
    delete[] pts;
    delete[] dists;

    // If you are performing range searches for the assignment, insert the
    // points on the appropriate data structure
    if( !queryDone && rangeAssign){
        queryDone = false;
        query->addVectorList(points);
    }

}

void LloydClusterer::assignment(Metric * metric){

    // This will pick the correct assignment algorithm according to the
    // settings that you applied

    // Reset the previous assignment before starting a new one
    clusters->reset();

    if(rangeAssign){
        rangeAssignment(metric);
    }else{
        directAssignment(metric);
    }

}

void LloydClusterer::directAssignment(Metric * metric){

    // This will compare distances with every centroid
    // and decide which one to put each point to

    // Assign every point
    for(int i = 0; i < points->getElems(); i++){

        // Get the point
        Vector * p = (Vector*) points->get(i);

        // Initialize the mindist and the mincluster
        int mincluster = 0;
        float mindist = metric->dist(p,centroids[0]);

        // Search for any smaller distances
        for(int j = 1; j < k; j++){
            
            // Calculate the new distance and check if its smaller
            float d = metric->dist(p,centroids[j]); 
            if (d < mindist){
                // Replace with the smaller one
                mindist = d;
                mincluster = j;
            }
        }

        // Finally, place the object on the cluster you found
        clusters->add((void*)p, mincluster);
        p->addPossibleCluster(mincluster,mindist);

    }

}

void LloydClusterer::rangeAssignment(Metric * metric){

    // This will perform range assignments using the data structure provided
    // The points have already been inserted on the initialization step

    // Vectors that have been clustered don't respond to range searches
    // So it is IMPORTANT to perform all the range queries first

    // Get some important variables down
    int els = points->getElems();
    int clustered_els = 0; // How many did you put

    // Pick the initial radius
    float radius = 5.0;

    // Create an array to hold the priority queues (as many as the clusters)
    PriorityQueue ** queues = new PriorityQueue*[k];

    while(true){

        // Perform a range search for all the centroids
        for(int i = 0; i < k; i++){
            queues[i] = query->approximateRange(radius, centroids[i],metric);
        }

        // Then cluster the elements of each search
        int now_clustered = 0;
        for(int i = 0; i < k; i++){
            
            // Find all the elements (priority is distance)
            QueueElement ** elems = queues[i]->getArray();
            
            // Try to cluster each element
            for(int j = 0; j < queues[i]->getElems(); j++){

                // Get the vector and the distance
                Vector * v = (Vector*) elems[j]->data;
                float d = elems[j]->priority;

                //Count the newly clustered elements
                if(v->addPossibleCluster(i,d)){
                    clustered_els++;
                    now_clustered++;
                }

            }

            // Finally dispose of the priority queue
            delete queues[i];

        }

        // Check if you need to stop the process
        // Terminal condition goes here
        if( clustered_els >= els || now_clustered == 0)
            break;

        // float the radius and begin anew
        radius *= 2;
        continue;

    }

    // Perform the actual clustering and check for leftovers
    for(int i = 0; i < els; i++){

        // Get each point to check/cluster
        Vector * v = (Vector *) points->get(i);

        // Find which cluster does it belong to
        int cluster = v->getCluster();
        if (cluster == -1){
            // If it does not belong, check every centroid manually
            float mindist = metric->dist(v,centroids[0]);
            cluster = 0;

            // Find the minimum through iteration
            for(int j = 1; j < k; j++){
                float d = metric->dist(v,centroids[j]);
                if(d < mindist){
                    mindist = d;
                    cluster = j;
                }
            }

            v->addPossibleCluster(cluster,mindist);
        }

        // Place it accordingly
        clusters->add((void*)v,cluster);


    }

    // After all that, dispose of the queue array
    delete[] queues;

}


bool LloydClusterer::update(){

    // This will update the centroids based on the points that are clustered

    // Initialize a temporary float array for computations
    float * coordsum = new float[dimension];
    float maxd = -1; // Calulates the maximum difference of position for centroids

    // Update every centroid
    for(int i = 0; i < k; i++){

        // Find the list of the points on the cluster
        List * clusterp = clusters->getChain(i);

        // Reset the sum for the coords
        for(int j = 0; j < dimension; j++)
            coordsum[j] = 0;
        
        // Run through the points to accumulate the sum
        int pno = clusterp->getElems();
        for(int j = 0; j < pno; j++){
            
            // Get the point
            Vector * p = (Vector *) ((HashElement*)clusterp->get(j))->data;

            // Add its coordinates to the sum
            for(int cc = 0; cc < dimension; cc++){
                coordsum[cc] += p->getCoord(cc);
            }

        }

        // After that, divide to get the actual coords
        // Compare with the old centroid position to find the terminal condition
        float movedist = 0;
        for(int j = 0; j < dimension; j++){
            
            // Do the division
            coordsum[j] /= pno;

            // Add the square difference
            float diff = coordsum[j]-centroids[i]->getCoord(j);
            movedist += diff*diff;

            // Replace the coordinate
            centroids[i]->setCoord(j, coordsum[j]);

        }

        // Keep the maximum movement distance
        if (maxd < movedist )
            maxd = movedist;

    }

    // Delete the coord array
    delete[] coordsum;

    // Check if you reached the threshold to stop
    float thres = 5.0;
    return maxd <= thres*thres && false;

}

int LloydClusterer::performClustering(Metric * metric,int times){

    // This will repeat the circle of clustering as many times
    // as the user commands. Clustering will stop automatically
    // because of the terminal conditions even if the cycles have
    // not yet finished.

    // First initialize the clustering
    initialization(metric);

    // Then perform the two step loop
    int i = 0;
    while(i != times){

        // Reset the clusters on each vector first
        for(int j = 0; j < points->getElems();j++){
            Vector * p = (Vector*) points->get(j);
            p->resetCluster();
        }

        assignment(metric);
        
        // Check if you need to stop after the update
        if ( update()){
            break;
        }

        i++;
    }

    return i;

}

List * LloydClusterer::getCluster(int c){

    // Returns the list of vectors that belong to a specific cluster
    
    // Check that the argument is within bounds
    if ( c < 0 || c > k)
        return nullptr;

    return clusters->getChain(c);

}

Vector * LloydClusterer::getCentroid(int c){

    // Returns the vector representing the centroid of the cluster
    // in question

    // Check that the argumetn is within bounds
    if (c < 0 || c > k)
        return nullptr;

    return centroids[c];

}

void LloydClusterer::calculateSillouete(Metric * metric){

    // Calculate for each vector and for each cluster

    // Initialize the temporary values
    int els = points->getElems();
    float a;
    float b;
    float s;

    // Reset the sillouete values
    for(int i = 0; i < k; i++)
        sillouete[i] = 0;

    int dots = 40;
    int dot_per = els/dots;
    int ddots = 0;


    // Perform for each vector
    for(int i = 0; i < els; i++){

        // Get the vector
        Vector * p = (Vector*) points->get(i);

        // Find its two closest clusters
        int first = p->getCluster(), second = -1;
        List * temp;
        float secondd = 0;
        for(int j = 0; j < k; j++){
            
            // Skip your cluster
            if(j == first) continue;
            
            // find the centroid distance
            float d = metric->dist(p,centroids[j]);

            // Replace accordingly
            if(second == -1 || secondd > d){
                second = j;
                secondd = d;
            }

        }

        // Fetch the clusters
        List * firstc = clusters->getChain(first), * secondc = clusters->getChain(second);

        // Find average distances
        a = 0;
        b = 0;
        for(int j = 0; j < firstc->getElems(); j++){
            // Find each other vector
            Vector* v = (Vector*)((HashElement*)firstc->get(j))->data;
            a += metric->dist(v,p);
        }
        //printf("a = %.2lf\n",a);
        a /= firstc->getElems();
        //printf("aa = %.2lf\n",a);

        for(int j = 0; j < secondc->getElems(); j++){
            // Find each other vector
            Vector* v = (Vector*)((HashElement*)secondc->get(j))->data;
            b += metric->dist(v,p);
        }
        //printf("b = %.2lf\n",b);
        b /= secondc->getElems();
        //printf("bb = %.2lf\n",b);

        // Calculate the s of the point
        s = (b-a)/((a>b)?a:b);
        //printf("s = %.2lf\n",s);

        // Add that to the silouete of the appropriate cluster
        sillouete[p->getCluster()] += s;


        if((i+1)/dot_per > ddots){
            ddots = (i+1)/dot_per;
            printf("\r[");
            for(int j = 0; j < dots; j++){
                if(j < ddots)
                    putchar('.');
                else
                    putchar(' ');
            }
            printf("] %d/%d",i+1,els);
            fflush(stdout);
        }

    }
    printf("\n");

    // At the end divide to get the correct results
    for(int i = 0; i < k; i++)
        sillouete[i] /= clusters->getChain(i)->getElems();


}

float LloydClusterer::getSillouete(int c){

    // This will return the sillouete of the specified cluster
    // You should calculate those first
    return sillouete[c];

}

float LloydClusterer::getGlobalSillouete(){

    // This will return the average sillouete for all the clusters
    float s_ret = 0;
    for(int i = 0; i < k; i++){
        s_ret += sillouete[i]*clusters->getChain(i)->getElems();
    }
    s_ret /= points->getElems();
    return s_ret;


}