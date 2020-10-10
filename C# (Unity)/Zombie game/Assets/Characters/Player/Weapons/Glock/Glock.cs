using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Glock : MonoBehaviour
{

    [SerializeField]
    private Camera cam;
    [SerializeField]
    private LayerMask mask;

    public Player player;

    ///////////////////////////////
    /// Differnt for each weapon //
    ///////////////////////////////
    public string name = "Glock";

    public static int damage = 8;
    public float range = 100f;
    public float fireRate = 10f;

    public int maxAmmo = 10;
    public int currentAmmo;
    public float reloadTime = 4f;
    private bool isReloading = false;

    /* stuff to add 
    public static float force = 10f;
    public static int max_ammo = 16;
    */

    ///////////////////////////////
    ///   Same for each weapon   //
    ///////////////////////////////


    void Start()
    {
        player = PlayerManager.instance.player1;
        currentAmmo = maxAmmo;
    }
    
    
    void Update()
    {
        if(Time.timeScale == 0f)
        {
            return;
        }
        if (isReloading)
        {
            return;
        }
        if (Input.GetButtonDown("Fire1"))
        {
            Shoot();
        }
    }
    void onEnable()
    {
        isReloading = false;

    }

    void Shoot()
    {

        currentAmmo--;
        if (currentAmmo <= 0)
        {
            isReloading = true;
            StartCoroutine(reload());
            return;
        }

        RaycastHit _hit;
        if (Physics.Raycast(cam.transform.position, cam.transform.forward, out _hit, range, mask))
        {
            //debugger, we hit something
            Debug.Log("hit " + _hit.collider.name);
            ZombieController target = _hit.transform.GetComponent<ZombieController>();
            if (target != null)
            {
                if (target.currentHealth - damage <= 0)
                {
                    player.UpdatePoints(50);
                }
                else
                {
                    player.UpdatePoints(10);
                }
                target.TakeDamage(damage);
                
            }
        }
    }

    IEnumerator reload()
    {
        Debug.Log("Reloading...");
        yield return new WaitForSeconds(reloadTime);
        currentAmmo = maxAmmo;
        isReloading = false;
    }

}




/*
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Glock : MonoBehaviour
{

    [SerializeField]
    private Camera cam;
    [SerializeField]
    private LayerMask mask;


    public string name = "Glock";

    public static int damage = 8;
    public float range = 100f;
    public float fireRate = 10f;
    public int maxAmmo = 10;
    private int currentAmmo;
    public float reloadTime = 4f;
    private bool isReloading = false;

    //stuff to add 
    //public static float force = 10f;
    //public static int max_ammo = 16;
    
    void start()
    {
        currentAmmo = maxAmmo;
    }
    void Update()
    {
        if (isReloading)
        {
            return;
        }
        if (Input.GetButtonDown("Fire1"))
        {
            Shoot();
        }
    }

    void onEnable()
    {
        isReloading = false;
        
    }

    void Shoot()
    {
        currentAmmo--;
        if (currentAmmo <= 0)
        {
            isReloading = true;
            StartCorutine(reload());
            return;
        }

        RaycastHit _hit;
        if (Physics.Raycast(cam.transform.position, cam.transform.forward, out _hit, range, mask))
        {
            //debugger, we hit something
            //Debug.Log("hit " + _hit.collider.name);
            
            
            ZombieAI target = _hit.transform.GetComponent<ZombieAI>();
            if (target != null)
            {
                target.TakeDamage(damage);
            }
        }

    }

    IEnumerator reload()
    {
        Debug.Log("Reloading...");
        yield return new WaitForSeconds(reloadTime);
        currentAmmo = maxAmmo;
        isReloading = false;
    }

}
*/
