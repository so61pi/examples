.. contents:: Table of Contents

Concepts
========

Pod
---

- Smallest management unit.
- Contains one or multiple containers.
- Pod templates specify how to create a pod.
- https://kubernetes.io/docs/concepts/workloads/pods/pod-overview/

Deployment
----------

- Manage ReplicaSets
- https://kubernetes.io/docs/concepts/workloads/controllers/deployment/
- Example

  .. code-block:: yaml

      apiVersion: apps/v1
      kind: Deployment
      metadata:
        name: nginx-deployment
        labels:
          app: nginx
      spec:
        replicas: 3
        selector:
          matchLabels:
            app: nginx
        template:
          metadata:
            labels:
              app: nginx
          spec:
            containers:
            - name: nginx
              image: nginx:1.7.9
              ports:
              - containerPort: 80

ReplicaSet
----------

- Ensure a specified number of pod replicas are running at any given time.
- Deployment should be preferred over ReplicaSet if possible.
- https://kubernetes.io/docs/concepts/workloads/controllers/replicaset/

DaemonSet
---------

- Make sure each node runs a copy of a Pod. For instance, log collector.
- https://kubernetes.io/docs/concepts/workloads/controllers/daemonset/
- Example

  .. code-block:: yaml

      apiVersion: apps/v1
      kind: DaemonSet
      metadata:
        name: fluentd-elasticsearch
        namespace: kube-system
        labels:
          k8s-app: fluentd-logging
      spec:
        selector:
          matchLabels:
            name: fluentd-elasticsearch
        template:
          metadata:
            labels:
              name: fluentd-elasticsearch
          spec:
            tolerations:
            - key: node-role.kubernetes.io/master
              effect: NoSchedule
            containers:
            - name: fluentd-elasticsearch
              image: quay.io/fluentd_elasticsearch/fluentd:v2.5.2
              resources:
                limits:
                  memory: 200Mi
                requests:
                  cpu: 100m
                  memory: 200Mi
              volumeMounts:
              - name: varlog
                mountPath: /var/log
              - name: varlibdockercontainers
                mountPath: /var/lib/docker/containers
                readOnly: true
            terminationGracePeriodSeconds: 30
            volumes:
            - name: varlog
              hostPath:
                path: /var/log
            - name: varlibdockercontainers
              hostPath:
                path: /var/lib/docker/containers

StatefulSet
-----------

- Used for stateful applications (e.g. databases) need ordered rollout or volumes.
- https://kubernetes.io/docs/concepts/workloads/controllers/statefulset/
- Example

  .. code-block:: yaml

      apiVersion: v1
      kind: Service
      metadata:
        name: nginx
        labels:
          app: nginx
      spec:
        ports:
        - port: 80
          name: web
        clusterIP: None
        selector:
          app: nginx
      ---
      apiVersion: apps/v1
      kind: StatefulSet
      metadata:
        name: web
      spec:
        selector:
          matchLabels:
            app: nginx # has to match .spec.template.metadata.labels
        serviceName: "nginx"
        replicas: 3 # by default is 1
        template:
          metadata:
            labels:
              app: nginx # has to match .spec.selector.matchLabels
          spec:
            terminationGracePeriodSeconds: 10
            containers:
            - name: nginx
              image: k8s.gcr.io/nginx-slim:0.8
              ports:
              - containerPort: 80
                name: web
              volumeMounts:
              - name: www
                mountPath: /usr/share/nginx/html
        volumeClaimTemplates:
        - metadata:
            name: www
          spec:
            accessModes: [ "ReadWriteOnce" ]
            storageClassName: "my-storage-class"
            resources:
              requests:
                storage: 1Gi

Service
-------

- Routing external traffics to correct endpoints.
- Endpoints can be extracted via selectors, or simply an IP:PORT destination.
- https://kubernetes.io/docs/concepts/services-networking/service/
- Example

  .. code-block:: yaml

      apiVersion: v1
      kind: Service
      metadata:
        name: my-service
      spec:
        selector:
          app: MyApp
        ports:
          - protocol: TCP
            port: 80
            targetPort: 9376

  .. code-block:: yaml

      apiVersion: v1
      kind: Endpoints
      metadata:
        name: my-service
      subsets:
        - addresses:
            - ip: 192.0.2.42
          ports:
            - port: 9376
      ---
      apiVersion: v1
      kind: Service
      metadata:
        name: my-service
      spec:
        ports:
          - protocol: TCP
            port: 80
            targetPort: 9376

Volume
------

- Mount an external volume into a directory of a container.
- Types

  * cephfs
  * iscsi
  * local
  * persistentVolumeClaim
  * ...

PersistentVolumeClaim
---------------------

- Claim a PersistentVolume without knowing underlying details (i.e. GCEPersistentDisk, AWSElasticBlockStore, NFS)
- Example

  .. code-block:: yaml

      apiVersion: v1
      kind: PersistentVolumeClaim
      metadata:
        name: myclaim
      spec:
        accessModes:
          - ReadWriteOnce
        volumeMode: Filesystem
        resources:
          requests:
            storage: 8Gi
        storageClassName: slow
        selector:
          matchLabels:
            release: "stable"
          matchExpressions:
            - {key: environment, operator: In, values: [dev]}
      ---
      apiVersion: v1
      kind: Pod
      metadata:
        name: mypod
      spec:
        containers:
          - name: myfrontend
            image: nginx
            volumeMounts:
            - mountPath: "/var/www/html"
              name: mypd
        volumes:
          - name: mypd
            persistentVolumeClaim:
              claimName: myclaim

PersistentVolume
----------------

- Hold the details of a volume.
- Utilized by PersistentVolumeClaim.
- https://kubernetes.io/docs/concepts/storage/persistent-volumes/
- Example

  .. code-block:: yaml

      apiVersion: v1
      kind: PersistentVolume
      metadata:
        name: pv0003
      spec:
        capacity:
          storage: 5Gi
        volumeMode: Filesystem
        accessModes:
          - ReadWriteOnce
        persistentVolumeReclaimPolicy: Recycle
        storageClassName: slow
        mountOptions:
          - hard
          - nfsvers=4.1
        nfs:
          path: /tmp
          server: 172.17.0.2

Namespace
---------

Resource Objects Reference
==========================

YAML syntax reference for Kubernetes resource objects ia located at https://kubernetes.io/docs/reference/generated/kubernetes-api/v1.17/

Networking
==========

- https://kubernetes.io/docs/concepts/services-networking/ingress/
